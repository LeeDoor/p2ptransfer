set(SOURCES 
    main.cpp
    test_file_processor.cpp
    test_general_presenter.cpp
    test_listener_presenter.cpp
    test_listener.cpp
)
if(NOT ${GRAPHICS})
set(SOURCES ${SOURCES}
    test_application.cpp
)
endif()
add_executable(server_tests ${SOURCES})
target_link_libraries(server_tests PUBLIC server ${LINK_LIBRARIES} GTest::gtest_main GTest::gmock)
target_precompile_headers(server_tests REUSE_FROM base)
target_include_directories(server_tests PUBLIC ${INCLUDE_DIRECTORIES})
target_compile_definitions(server_tests PUBLIC TESTING)
if (UNIX)
    gtest_discover_tests(server_tests)
endif()
