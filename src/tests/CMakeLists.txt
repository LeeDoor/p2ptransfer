set(SOURCES 
    main.cpp
    test_file_reader.cpp
    test_general_presenter.cpp
    test_listener_presenter.cpp
    test_listener.cpp
    test_request_serializer.cpp
    test_request_deserializer.cpp
    test_socket_manager.cpp
    test_address_gatherer.cpp
    test_thread_wrapper.cpp
)
if(${GRAPHICS})
    set(TEST_NAME gui_tests)
else()
    set(TEST_NAME cli_tests)
endif()
add_executable(${TEST_NAME} ${SOURCES})
target_link_libraries(${TEST_NAME} PUBLIC ${LINK_LIBRARIES} server pch_tests GTest::gtest_main GTest::gmock)
target_include_directories(${TEST_NAME} PUBLIC ${INCLUDE_DIRECTORIES})
target_compile_definitions(${TEST_NAME} PUBLIC TESTING)

set(TEST_ARGS "--gtest_brief" "--gtest_break_on_failure")
add_test(NAME GUI_test COMMAND gui_tests ${TEST_ARGS})
add_test(NAME CLI_test COMMAND cli_tests ${TEST_ARGS})
# TODO: remove, if other configuration works
#if (UNIX)
#    gtest_discover_tests(${TEST_NAME})
#endif()
