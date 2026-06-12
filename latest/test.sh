#!/bin/bash
ECHO 

echo "INITIALIZING DATABASE"

sudo -u postgres psql \
    -d shotgun \
    -f database/init.sql \
    > /dev/null

if [ $? -ne 0 ]; then
    echo "DATABASE INIT FAILED"
    exit 1
fi

make -f makefile.mk


if [ $? -ne 0 ]; then
    echo "COMPILE FAILED"
    exit 1
fi

echo "PROGRAM COMPILED"
echo "RUNNING THE SERVER"

./shotgun &
SERVER_PID=$!

echo "Server PID: $SERVER_PID"

newman run postman/shotgun.postman_collection.json --env-var "host=localhost"

echo "KILLING SERVER"
kill $SERVER_PID


