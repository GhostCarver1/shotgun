make rebuild_test || true

if [ $? -ne 0 ]; then
    echo "COMPILE FAILED"
    exit 1
fi

echo "TEST COMPILED SUCCESSFULLY"

echo "RUNNING UNIT TESTS"

if ./test; then
    echo "Tests passed"
else
    echo "Tests failed"
fi