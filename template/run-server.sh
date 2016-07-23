#!/bin/sh

set x

echo "kill current server"
killall server

nohup ./server/server ./conf/server.conf &

