cd ../build/x86_64_linux/

if [ -d cli ]; then
    ctest --test-dir cli --output-on-failure --stop-on-failure $@
elif [ -d graphics ]; then
    ctest --test-dir graphics --output-on-failure --stop-on-failure $@
else
    echo "cant find a directory to run tests here: $(pwd)"
fi
