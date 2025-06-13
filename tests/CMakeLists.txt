find_package(GTest REQUIRED)
enable_testing()
add_executable(tests
    test_presenter.cpp
)
target_precompile_headers(tests INTERFACE test_headers.hpp)
target_link_libraries(tests PUBLIC server GTest::gtest_main GTest::gmock ${LINKAGE_PARAMS})
gtest_discover_tests(tests)
