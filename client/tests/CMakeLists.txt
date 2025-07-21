set(SOURCES 
    main.cpp

    test_presenter.cpp
)

add_executable(client_tests ${SOURCES})

target_link_libraries(client_tests PUBLIC client ${LINK_LIBRARIES} GTest::gtest_main GTest::gmock)
target_precompile_headers(client_tests REUSE_FROM base)
target_include_directories(client_tests PUBLIC ${INCLUDE_DIRECTORIES})
if (UNIX)
    gtest_discover_tests(client_tests)
endif()
