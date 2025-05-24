find_package(Catch2 3 REQUIRED)
add_executable(tests
    test_argument_parser.cpp
)
target_link_libraries(tests PUBLIC server Catch2::Catch2WithMain ${LINKAGE_PARAMS})
