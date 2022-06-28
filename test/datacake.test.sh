#!/bin/bash

device=$DEVICE_NAME
URL=$DATACAKE_URL
lat=$LAT
lon=$LON

# Function to generate random lightning detection
function genJSON() {
    local distance=$(($RANDOM % 40))
    local energy=$(($RANDOM % 20))

    # return json string
    echo '{"device":"'$device'","best_lat":'$lat',"best_lon":'$lon',"body":{"distance":'$distance',"energy":'$energy',"message":"Lightning detected"}}'
}

function main() {
    # Loop continuously, sending POST request to datacake
    while true; do
        curl -s -X POST -H "Content-Type: application/json" -d "$(genJSON)" $URL
        echo ""
        sleep 5
    done
}

main
