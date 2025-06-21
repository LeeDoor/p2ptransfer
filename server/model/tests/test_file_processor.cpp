#include "file_processor.hpp"
#include "gtest_headers.hpp"
#include "remote_interaction_callback_mock.hpp"
#include "socket_manager_mock.hpp"
#include "request_serializer.hpp"

constexpr size_t TEST_PORT = 8080;
constexpr std::string TEST_ADDRESS = "127.0.0.1";
constexpr std::string TEST_FILENAME = "test_file.txt";
const std::string TEST_FILE_CONTENT = "This is the test file@@@ content??? !!! \n\n";
const size_t TEST_FILESIZE = TEST_FILE_CONTENT.size();

class FileProcessorFixture : public ::testing::Test {
protected:
    FileProcessorFixture() :
        socket_mock(std::make_shared<SocketManagerMock>()),
        callback_mock(std::make_shared<RemoteInteractionCallbackMock>()),
        file_processor(socket_mock)    
    {
        file_processor.set_callback(callback_mock);
    }

    void immitate_send_request() {
        EXPECT_CALL(*socket_mock, read_request())
            .WillOnce(Return(return_immediately(
                RequestSerializer::serialize_send_request(TEST_FILENAME, TEST_FILESIZE))));
    }
    void immitate_user_verification() {
        EXPECT_CALL(*callback_mock, verify_file(TEST_FILENAME, TEST_FILESIZE))
            .WillOnce(Return(true));
    }
    void check_response_sending() {
        EXPECT_CALL(*socket_mock, send_response(RequestSerializer::serialize_send_permission(TEST_FILENAME)))
            .WillOnce(Return(return_immediately()));
    }
    void immitate_file_content_sending() {
        size_t bytes_remaining = TEST_FILESIZE;
        EXPECT_CALL(*socket_mock, read_file_part_to(testing::_, bytes_remaining))
            .WillOnce(testing::Invoke([](ISocketManager::BufferType& buffer, size_t& bytes_remaining) {
                std::copy(TEST_FILE_CONTENT.begin(), TEST_FILE_CONTENT.end(), buffer.begin());
                bytes_remaining = 0;
                return return_immediately(TEST_FILESIZE);
            }));    
    }
    void check_progressbar_callbacks() {
        EXPECT_CALL(*callback_mock, set_progressbar(::testing::_))
            .Times(::testing::AtLeast(1));
    }
    void check_file_transfered_callback() {
        EXPECT_CALL(*callback_mock, file_transfered());
    }
    void run_read_file() {
        net::io_context context;
        net::co_spawn(context, file_processor.try_read_file(), [](std::exception_ptr ptr) {
            if(ptr) std::rethrow_exception(ptr);
        });
        context.run();
    }
    void verify_file_content() {
        std::ifstream ifstream("READED_" + TEST_FILENAME);
        ASSERT_TRUE(ifstream.is_open());
        EXPECT_EQ(std::filesystem::file_size("READED_" + TEST_FILENAME), TEST_FILESIZE);
        std::stringstream ss;
        ss << ifstream.rdbuf();
        EXPECT_EQ(ss.str(), TEST_FILE_CONTENT);
    }

    std::shared_ptr<SocketManagerMock> socket_mock;
    std::shared_ptr<RemoteInteractionCallbackMock> callback_mock;
    FileProcessor file_processor;
};

TEST_F(FileProcessorFixture, file_readed_successfully) {
    immitate_send_request();
    immitate_user_verification();
    check_response_sending();
    immitate_file_content_sending();
    check_progressbar_callbacks();
    check_progressbar_callbacks();
    EXPECT_NO_THROW(run_read_file());
    verify_file_content();
}
