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
    test_transferer.cpp
)
set(TEST_NAME generic_tests)
add_executable(${TEST_NAME} ${SOURCES})
target_link_libraries(${TEST_NAME} PUBLIC ${LINK_LIBRARIES} 
    server_model 
    server_presenter 
    pch_tests 
    GTest::gtest_main 
    GTest::gmock)
target_include_directories(${TEST_NAME} PUBLIC 
    ${INCLUDE_DIRECTORIES}
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
)
target_compile_definitions(${TEST_NAME} PUBLIC TESTING)

set(TEST_ARGS "--gtest_brief" "--gtest_break_on_failure")
add_test(NAME generic COMMAND ${TEST_NAME} ${TEST_ARGS})
