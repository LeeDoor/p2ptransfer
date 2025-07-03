cd ../build/
ctest --test-dir x86_64_linux/cli --output-on-failure --stop-on-failure $@
ctest --test-dir x86_64_linux/graphics --output-on-failure --stop-on-failure $@
