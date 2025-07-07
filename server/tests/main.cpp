int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    
    ::testing::FLAGS_gmock_default_mock_behavior = 2;
    ::testing::FLAGS_gtest_stack_trace_depth = 10;
    
    return RUN_ALL_TESTS();
}
