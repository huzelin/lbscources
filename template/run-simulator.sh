#!/bin/sh
set x

echo "kill current simulator"
killall simulator

echo "run simulator"
nohup ./simulator/simulator ./conf/simulator.conf &


