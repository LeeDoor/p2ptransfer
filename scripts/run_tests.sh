./build.sh 0
ctest --test-dir ../build/$(uname) \
    --timeout 4 \
    --output-on-failure \
    --stop-on-failure \
    $@

