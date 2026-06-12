#!/bin/bash
echo "STARTING UP THE SERVER"

echo "INITIALIZING DATABASE"

set -a
source env.run
set +a

sudo service postgresql start

sudo -u postgres psql \
    -d shotgun \
    -f database/init.sql \
    > /dev/null

if [ $? -ne 0 ]; then
    echo "DATABASE INIT FAILED"
    exit 1
fi


rm -f shotgun
make clean || true
make -f makefile.mk


if [ $? -ne 0 ]; then
    echo "COMPILE FAILED"
    exit 1
fi

echo "PROGRAM COMPILED"
echo "RUNNING THE SERVER"
./shotgun


