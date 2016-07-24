#!/bin/sh
set x

PID=`pidof server`

if [ -z $PID ]; then
	echo "server is not running"
else
	killall server
	echo "simulator is killed"
fi


