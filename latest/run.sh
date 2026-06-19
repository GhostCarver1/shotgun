#!/bin/bash
set -e 

if lsof -i :8080 > /dev/null 2>&1; then
    echo "Killing old server"
    kill $(lsof -t -i:8080)
else 
    echo "NO SERVER TO KILL"
fi

echo "SETTING SOURCE TO ENV.RUN"

set -a
source env.run
set +a

echo "STARTING POSTRGRESS"

sudo service postgresql start

echo "INITIALIZING DATABASE"

sudo -u postgres psql \
    -d shotgun \
    -f database/init.sql \
    > /dev/null

if [ $? -ne 0 ]; then
    echo "DATABASE INIT FAILED"
    exit 1
fi

make rebuild || true

if [ $? -ne 0 ]; then
    echo "COMPILE FAILED"
    exit 1
fi

echo "PROGRAM COMPILED"
echo "RUNNING THE SERVER"
./shotgun


