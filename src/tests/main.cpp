int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    
    ::testing::FLAGS_gmock_default_mock_behavior = 0;
    
    return RUN_ALL_TESTS();
}
