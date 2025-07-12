add_executable(general_tests 
    main.cpp

    test_request_serializer.cpp
    test_request_deserializer.cpp
    test_socket_manager.cpp
    test_address_gatherer.cpp
    test_thread_wrapper.cpp
)
target_link_libraries(general_tests PUBLIC server ${LINK_LIBRARIES} GTest::gtest_main GTest::gmock)
target_precompile_headers(general_tests REUSE_FROM base)
target_include_directories(general_tests PUBLIC ${INCLUDE_DIRECTORIES})
if (UNIX)
    gtest_discover_tests(general_tests)
endif()
