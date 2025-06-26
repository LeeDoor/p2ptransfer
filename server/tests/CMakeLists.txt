add_executable(tests 
    model_mock_factory.cpp
    view_mock_factory.cpp

    test_connection_establisher.cpp
    test_file_processor.cpp
    test_presenter.cpp
    test_listener.cpp
)
target_link_libraries(tests PUBLIC server ${LINK_LIBRARIES} test_headers GTest::gtest_main GTest::gmock)
target_include_directories(tests PUBLIC ${INCLUDE_DIRECTORIES})
gtest_discover_tests(tests)
