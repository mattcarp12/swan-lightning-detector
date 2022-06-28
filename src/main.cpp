#include <SPI.h>
#include <Wire.h>
#include "SparkFun_AS3935.h"
#include <Notecard.h>

#define myProductID "com.blues.mcarpenter:swan_lightning_detector"
Notecard notecard;

// 0x03 is default, but the address can also be 0x02, 0x01.
// Adjust the address jumpers on the underside of the product.
#define AS3935_ADDR 0x03
#define INDOOR 0x12
#define OUTDOOR 0xE
#define LIGHTNING_INT 0x08
#define DISTURBER_INT 0x04
#define NOISE_INT 0x01

SparkFun_AS3935 lightning(AS3935_ADDR);

// Interrupt pin for lightning detection
const int lightningInt = 5;


// This variable holds the number representing the lightning or non-lightning
// event issued by the lightning detector.
int intVal = 0;
int noise = 4;     // Value between 1-7
int disturber = 3; // Value between 1-10
int watchdog = 4;  // Value between 1-10

void sendNotecardMsg(const char *);
void sendLigntningMsg(byte, uint32_t);

void setup()
{
    // When lightning is detected the interrupt pin goes HIGH.
    pinMode(lightningInt, INPUT);

    Wire.begin(); // Begin Wire before lightning sensor.
    notecard.begin();

    if (!lightning.begin())
    { // Initialize the sensor.
        notecard.logDebug("AS3935 initialization failed");
        while (1)
            ;
    }
    else
        // sendNotecardMsg("AS3935 initialization successful");
        notecard.logDebug("AS3935 initialization successful");

    // The lightning detector defaults to an indoor setting at
    // the cost of less sensitivity, if you plan on using this outdoors
    // uncomment the following line:
    lightning.setIndoorOutdoor(OUTDOOR);

    // lightning.watchdogThreshold(watchdog);
    // lightning.calibrateOsc();
    // lightning.maskDisturber(1);
}

void loop()
{
    
    if (digitalRead(lightningInt) == HIGH)
    {
        // Hardware has alerted us to an event, now we read the interrupt register
        // to see exactly what it is.
        intVal = lightning.readInterruptReg();
        if (intVal == NOISE_INT)
        {
            sendNotecardMsg("Noise detected");
            notecard.logDebug("Noise detected");
            // Too much noise? Uncomment the code below, a higher number means better
            // noise rejection.
            lightning.setNoiseLevel(noise);
        }
        else if (intVal == DISTURBER_INT)
        {
            sendNotecardMsg("Disturber detected");
            notecard.logDebug("Disturber detected");
            // Too many disturbers? Uncomment the code below, a higher number means better
            // disturber rejection.
            // lightning.watchdogThreshold(threshVal);
        }
        else if (intVal == LIGHTNING_INT)
        {
            sendNotecardMsg("Lightning detected");
            notecard.logDebug("Lightning detected");
            // Lightning! Now how far away is it? Distance estimation takes into
            // account any previously seen events in the last 15 seconds.
            byte distance = lightning.distanceToStorm();
            uint32_t energy = lightning.lightningEnergy();
            sendLigntningMsg(distance, energy);
        }
    }
    delay(100); // Slow it down.
}

void sendNotecardBody(J *body)
{
    // Enqueue the message to the Notecard for transmission to the Notehub, adding the "sync"
    // flag for demonstration purposes to upload the data instantaneously, so that if you are looking
    // at this on notehub.io you will see the data appearing 'live'.)
    J *req = notecard.newRequest("note.add");
    if (req != NULL)
    {
        JAddBoolToObject(req, "sync", true);
        JAddItemToObject(req, "body", body);
        notecard.sendRequest(req);
    }
}

void sendNotecardMsg(const char *message)
{
    J *body = JCreateObject();
    if (body != NULL)
    {
        JAddStringToObject(body, "message", message);
    }
    sendNotecardBody(body);
}

void sendLigntningMsg(byte distance, uint32_t energy)
{
    J *body = JCreateObject();
    if (body != NULL)
    {
        JAddStringToObject(body, "message", "Lightning Strike Detected!");
        JAddNumberToObject(body, "distance", distance);
        JAddNumberToObject(body, "energy", energy);
    }
    sendNotecardBody(body);
}
