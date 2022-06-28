function Decoder(request) {
    
    var data = JSON.parse(request.body);
    var device = data.device;
    
    var file = data.file;
    
    var decoded = {};
    
    if (file === "locations.qos") {
        
        decoded.voltage = data.body.voltage;
        decoded.motion = data.body.motion;
        decoded.seconds = data.body.seconds;
        
    } else if (file === "_session.qo") {
        
        decoded.voltage = data.voltage;
    }
    
    if (("tower_lat" in data) && ("tower_lon" in data)) {
        decoded.tower_location = "(" + data.tower_lat + "," + data.tower_lon + ")";
    }
    if (("best_lat" in data) && ("best_lon" in data)) {
        decoded.device_location = "(" + data.best_lat + "," + data.best_lon + ")";
    }
    
    decoded.distance = data.body.distance;
    decoded.energy = data.body.energy;
    decoded.message = data.body.message

    return [
        {
            device: device,
            field: "TOWER_LOCATION",
            value: decoded.tower_location
        },
        {
            device: device,
            field: "DEVICE_LOCATION",
            value: decoded.device_location
        },        
        {
            device: device,
            field: "DISTANCE",
            value: decoded.distance
        },
        {
            device: device,
            field: "ENERGY",
            value: decoded.energy
        }
    ];
    
}