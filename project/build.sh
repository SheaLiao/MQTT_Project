#!/bin/bash 


export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/home/iot24/liaoshengli/mqtt_project/project/lib/ 

./mqtt -t 5 -d
