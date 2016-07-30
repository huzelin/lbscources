#!/bin/sh

set x

echo "kill current server"
sh stop-server.sh

nohup ./server/server ./conf/server.conf & 

