#!/bin/bash

set -euo pipefail

# set the image name from the parameters
IMGNAME=$1
FULL_IMAGE=$2

# execute the model in the debug setting
build/platforms/armv8_minimal/isim_system --data Memory0=${IMGNAME}@0x0 -S &



# record the pid
PID=$!
sleep 1
# modeldebugger --list-connections
modeldebugger -c 7000 --application ${FULL_IMAGE} -y

# terminate the simulation
kill -9 ${PID}