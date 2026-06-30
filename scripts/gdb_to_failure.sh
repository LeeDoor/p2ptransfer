./build.sh 0 && \
gdb --args ../build/Linux/src/tests/generic_tests --gtest_break_on_failure $@
