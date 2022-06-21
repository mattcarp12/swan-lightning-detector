#!/bin/bash

device=dev:94deb8230282
URL=https://api.datacake.co/integrations/api/77be0eb7-06e3-46cd-b714-6a3247b58e28/
lat=27.97087589
lon=-82.31997608

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
