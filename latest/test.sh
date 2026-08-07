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

echo "REBUILDING TEST FILES AND REGULAR FILES AND RUNNING"

make rebuild_test || true
if [ $? -ne 0 ]; then
    echo "TEST COMPILE FAILED"
    exit 1
fi

make rebuild || true
if [ $? -ne 0 ]; then
    echo "MAIN COMPILE FAILED"
    exit 1
fi

echo "PROGRAM TEST AND MAIN COMPILED"

if [[ ${1:-0} -ge 1 ]]; then
    echo "RUNNING UNIT TESTS"
    if ! ./test; then
        echo "TESTS FAILED"
        exit
    else
        echo "TESTS PASSED"
    fi
fi

if [[ ${1:-0} -ge 2  ]]; then
    echo "STARTING UP THE SERVER"
    ./shotgun 1 & SERVER_PID=$!
    echo "SERVER PID: $SERVER_PID"
fi

if [[ ${1:-0} -ge 3  ]]; then
    echo "COMPLETING POSTMAN FAST TESTS"
    sleep 2
    newman run postman/shotgun.postman_collection.json --env-var "host=localhost"
fi

if [[ ${1:-0} -ge 4  ]]; then
    echo "COMPLETING POSTMAN SLOW TESTS"
    sleep 2
    newman run postman/shotgun.postman_collection.json --env-var "host=localhost"
fi

if [[ ${2:-0} -ge 1  ]]; then
    echo "NOT KILLING THE SERVER"
else 
    echo "KILLING SERVER"
    kill $SERVER_PID
fi 

