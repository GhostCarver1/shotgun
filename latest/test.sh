#!/bin/bash
set -e

echo "INITIALIZING DATABASE"

set -a
source env.test
set +a

sudo service postgresql start

sudo -u postgres psql -c "CREATE ROLE shotgun_admin LOGIN PASSWORD 'password';" || true

sudo -u postgres psql -c "DROP DATABASE IF EXISTS shotgun_test;"
sudo -u postgres createdb shotgun_test -O shotgun_admin

sudo -u postgres psql -d shotgun_test -f database/init.sql > /dev/null
sudo -u postgres psql -d shotgun_test -f database/test_init.sql > /dev/null

rm -f shotgun
make clean || true
make -f makefile.mk

echo "PROGRAM COMPILED"
echo "RUNNING THE SERVER"

./shotgun &
SERVER_PID=$!

echo "Server PID: $SERVER_PID"

sleep 2

newman run postman/shotgun.postman_collection.json --env-var "host=localhost"

echo "KILLING SERVER"
kill $SERVER_PID