#include "gtest_headers.hpp"
#include "request_deserializer.hpp"

TEST(RequestDeserializer, SuccessSerializing_avg) {
    std::string request = 
        "REQUEST\n"
        "FILE aboba.txt\n"
        "SIZE 123\n"
        "\n";

    auto result = RequestDeserializer::deserialize_send_request(request);
    
    EXPECT_EQ(result.filename, "aboba.txt");
    EXPECT_EQ(result.filesize, 123);
}
TEST(RequestDeserializer, SuccessSerializing_space_filename) {
    std::string request = 
        "REQUEST\n"
        "FILE file with spaces.txt\n"
        "SIZE 0\n"
        "\n";

    auto result = RequestDeserializer::deserialize_send_request(request);
    
    EXPECT_EQ(result.filename, "file with spaces.txt");
    EXPECT_EQ(result.filesize, 0);
}
TEST(RequestDeserializer, SuccessSerializing_leading_space_filename) {
    std::string request = 
        "REQUEST\n"
        "FILE     leading spaces.txt\n"
        "SIZE 123\n"
        "\n";

    auto result = RequestDeserializer::deserialize_send_request(request);
    
    EXPECT_EQ(result.filename, "    leading spaces.txt");
    EXPECT_EQ(result.filesize, 123);
}
TEST(RequestDeserializer, SuccessSerializing_leading_and_trailing_spaces) {
    std::string request = 
        "REQUEST\n"
        "FILE     bothsize spaces.txt       \n"
        "SIZE 123\n"
        "\n";

    auto result = RequestDeserializer::deserialize_send_request(request);
    
    EXPECT_EQ(result.filename, "    bothsize spaces.txt       ");
    EXPECT_EQ(result.filesize, 123);
}
TEST(RequestDeserializer, SuccessSerializing_no_extention_filename) {
    std::string request = 
        "REQUEST\n"
        "FILE no_extension\n"
        "SIZE 123\n"
        "\n";

    auto result = RequestDeserializer::deserialize_send_request(request);
    
    EXPECT_EQ(result.filename, "no_extension");
    EXPECT_EQ(result.filesize, 123);
}
TEST(RequestDeserializer, SuccessSerializing_no_extention_with_spaced) {
    std::string request = 
        "REQUEST\n"
        "FILE    no extension spacees    \n"
        "SIZE 123\n"
        "\n";

    auto result = RequestDeserializer::deserialize_send_request(request);
    
    EXPECT_EQ(result.filename, "   no extension spacees    ");
    EXPECT_EQ(result.filesize, 123);
}
TEST(RequestDeserializer, SuccessSerializing_max_filesizesize) {
    std::string request = 
        "REQUEST\n"
        "FILE aboba.txt\n"
        "SIZE " + std::to_string(SIZE_MAX) + "\n"
        "\n";

    auto result = RequestDeserializer::deserialize_send_request(request);
    
    EXPECT_EQ(result.filename, "aboba.txt");
    EXPECT_EQ(result.filesize, SIZE_MAX);
}
