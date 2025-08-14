#include "request_serializer.hpp"
#include "request_structures.hpp"

namespace general {
namespace test {

TEST(RequestSerializer, serializeRequest_defaultData) {
    std::string result = RequestSerializer::serialize_send_request("filename", 123);

    ASSERT_EQ(result, 
              "REQUEST\n"
              "FILE filename\n"
              "SIZE 123\n\n");
}

TEST(RequestSerializer, serializeRequest_spacedFilename) {
    std::string result = RequestSerializer::serialize_send_request("   filename with spaces   ", 123);

    ASSERT_EQ(result, 
              "REQUEST\n"
              "FILE    filename with spaces   \n"
              "SIZE 123\n\n");
}

TEST(RequestSerializer, serializeRequest_hugeFilesize) {
    std::string result = RequestSerializer::serialize_send_request("filename", std::numeric_limits<Filesize>::max());

    ASSERT_EQ(result, 
              "REQUEST\n"
              "FILE filename\n"
              "SIZE " + std::to_string(std::numeric_limits<Filesize>::max()) + "\n\n");
}

TEST(RequestSerializer, serializeRequest_emptyString) {
    std::string result = RequestSerializer::serialize_send_request("", 123);

    ASSERT_EQ(result, 
              "REQUEST\n"
              "FILE \n"
              "SIZE 123\n\n");
}

TEST(RequestSerializer, serializeRequest_0filesize) {
    std::string result = RequestSerializer::serialize_send_request("filename", 0);

    ASSERT_EQ(result, 
              "REQUEST\n"
              "FILE filename\n"
              "SIZE 0\n\n");
}

TEST(RequestSerializer, serializePermission_defaultData) {
    std::string result = RequestSerializer::serialize_send_permission("filename");

    ASSERT_EQ(result, 
              "PERMISSION\n"
              "FILE filename\n\n");
}

TEST(RequestSerializer, serializePermission_withSpaces) {
    std::string result = RequestSerializer::serialize_send_permission("   filename   ");

    ASSERT_EQ(result, 
              "PERMISSION\n"
              "FILE    filename   \n\n");
}

TEST(RequestSerializer, serializePermission_empty) {
    std::string result = RequestSerializer::serialize_send_permission("");

    ASSERT_EQ(result, 
              "PERMISSION\n"
              "FILE \n\n");
}

}
}
