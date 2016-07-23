#!/bin/sh

set x

echo "kill current server"
killall server

./server/server ./conf/server.conf 

