#!/bin/bash
echo "INITIALIZING DATABASE"

sudo -u postgres psql \
    -d shotgun \
    -f database/init.sql \
    > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo "DATABASE INIT FAILED"
    exit 1
fi

gcc server/main.c server/handlers/*.c database/database.c \
    -I/usr/include/postgresql \
    -o shotgun \
    -lpq \
    -lsodium


if [ $? -ne 0 ]; then
    echo "COMPILE FAILED"
    exit 1
fi

echo "PROGRAM COMPILED"
echo "RUNNING THE SERVER"
./shotgun


