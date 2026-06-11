#!/bin/bash
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
./shotgun


