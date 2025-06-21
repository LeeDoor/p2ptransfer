#include "file_processor.hpp"
#include "gtest_headers.hpp"
#include "remote_interaction_callback_mock.hpp"
#include "socket_manager_mock.hpp"
#include "request_serializer.hpp"

class FileProcessorFixture : public ::testing::Test {
protected:
    FileProcessorFixture() :
        socket_mock(std::make_shared<SocketManagerMock>()),
        callback_mock(std::make_shared<RemoteInteractionCallbackMock>()),
        file_processor(socket_mock)    
    {
        file_processor.set_callback(callback_mock);
    }

    std::shared_ptr<SocketManagerMock> socket_mock;
    std::shared_ptr<RemoteInteractionCallbackMock> callback_mock;
    FileProcessor file_processor;
};

TEST_F(FileProcessorFixture, file_readed_successfully) {
    EXPECT_CALL(*socket_mock, read_request())
        .WillOnce(Return(return_immediately(RequestSerializer::serialize_send_request(TEST_FILENAME, TEST_FILESIZE))));
    EXPECT_CALL(*callback_mock, verify_file(TEST_FILENAME, TEST_FILESIZE))
        .WillOnce(Return(true));

    EXPECT_CALL(*socket_mock, send_response(RequestSerializer::serialize_send_permission(TEST_FILENAME)))
        .WillOnce(Return(return_immediately()));

    size_t bytes_remaining = TEST_FILESIZE;
    EXPECT_CALL(*socket_mock, read_file_part_to(testing::_, bytes_remaining))
        .WillOnce(testing::Invoke([](ISocketManager::BufferType& buffer, size_t& bytes_remaining) {
            std::copy(TEST_FILE_CONTENT.begin(), TEST_FILE_CONTENT.end(), buffer.begin());
            bytes_remaining = 0;
            return return_immediately(TEST_FILESIZE);
        }));    
    EXPECT_CALL(*callback_mock, set_progressbar(::testing::_))
        .Times(::testing::AtLeast(1));
    EXPECT_CALL(*callback_mock, file_transfered());

    net::io_context context;
    net::co_spawn(context, file_processor.try_read_file(), [](std::exception_ptr ptr) {
        if(ptr) std::rethrow_exception(ptr);
    });
    EXPECT_NO_THROW({
        context.run();
    });
    std::ifstream ifstream("READED_" + TEST_FILENAME);
    ASSERT_TRUE(ifstream.is_open());
    EXPECT_EQ(std::filesystem::file_size("READED_" + TEST_FILENAME), TEST_FILESIZE);
    std::stringstream ss;
    ss << ifstream.rdbuf();
    EXPECT_EQ(ss.str(), TEST_FILE_CONTENT);
}
