add_executable(general_tests 
    test_request_serializer.cpp
    test_request_deserializer.cpp
)
target_link_libraries(general_tests PUBLIC server ${LINK_LIBRARIES} GTest::gtest_main GTest::gmock)
target_precompile_headers(general_tests REUSE_FROM base)
target_include_directories(general_tests PUBLIC ${INCLUDE_DIRECTORIES})
if (UNIX)
    gtest_discover_tests(general_tests)
endif()
