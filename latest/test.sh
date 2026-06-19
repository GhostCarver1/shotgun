#!/bin/bash
set -e

echo "CHECKING IF SERVER IS ALREADY RUNNING"

if lsof -i :8080 > /dev/null 2>&1; then
    echo "KILLING OLD SERVER"
    kill $(lsof -t -i:8080)
else 
    echo "NO SERVER TO KILL"
fi

echo "SETTING SOURCE TO ENV.TEST"

set -a
source env.test
set +a

echo "STARTING POSTRGRESS"

sudo service postgresql start

echo "CREATING TEST DATABASE"
sudo -u postgres psql -c "CREATE ROLE shotgun_admin LOGIN PASSWORD 'password';" || true
sudo -u postgres psql -c "DROP DATABASE IF EXISTS shotgun_test;"
sudo -u postgres createdb shotgun_test -O shotgun_admin


echo "INITIALIZING DATABASE"
sudo -u postgres psql -d shotgun_test -f database/init.sql > /dev/null

echo "POPULATING DATABASE WITH TEST VALUES"
sudo -u postgres psql -d shotgun_test -f database/test_init.sql > /dev/null

echo "REBUILDING RUN FILES AND COMPILING"

make rebuild || true

if [ $? -ne 0 ]; then
    echo "COMPILE FAILED"
    exit 1
fi

echo "PROGRAM COMPILED"

echo "RUNNING UNIT TESTS"

echo "RUNNING THE SERVER IN THE BACKGROUND"

./shotgun &
SERVER_PID=$!

echo "SERVER PID: $SERVER_PID"

echo "LETTING THE SERVER WAKE UP BEFORE POSTMAN TESTS"

sleep 1
echo "."
sleep 1
echo "."
sleep 1
echo "."
sleep 1
echo "."
sleep 1

echo "RUNNING POSTMAN TESTS"

newman run postman/shotgun.postman_collection.json --env-var "host=localhost"

if [[ "$1" == "1" ]]; then
    echo "NOT KILLING THE SERVER"
else 
    echo "KILLING SERVER"
    kill $SERVER_PID
fi 

