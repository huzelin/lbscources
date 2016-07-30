#!/bin/sh
set x

PID=`pidof simulator`

if [ -z $PID ]; then
	echo "simulator is not running"
else
	killall simulator
	echo "simulator is killed"
fi


