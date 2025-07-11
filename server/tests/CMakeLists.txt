add_executable(server_tests 
    main.cpp

    test_file_processor.cpp
    test_presenter.cpp
    test_listener.cpp
    test_socket_manager.cpp
    test_address_gatherer.cpp
    test_thread_wrapper.cpp
)
target_link_libraries(server_tests PUBLIC server ${LINK_LIBRARIES} GTest::gtest_main GTest::gmock)
target_precompile_headers(server_tests REUSE_FROM base)
target_include_directories(server_tests PUBLIC ${INCLUDE_DIRECTORIES})
if (UNIX)
    gtest_discover_tests(server_tests)
endif()
