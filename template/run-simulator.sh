#!/bin/sh
set x


sh stop-simulator.sh

echo "run simulator"
nohup ./simulator/simulator ./conf/simulator.conf &


