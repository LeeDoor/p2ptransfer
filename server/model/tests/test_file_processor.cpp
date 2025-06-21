#include "connection_establisher.hpp"
#include "file_processor.hpp"
#include "remote_interaction_callback_mock.hpp"
#include "request_serializer.hpp"
#include "socket_manager_mock.hpp"
#include "gtest_headers.hpp"

using ::testing::Return;

constexpr size_t TEST_PORT = 8080;
constexpr std::string TEST_ADDRESS = "127.0.0.1";
constexpr std::string TEST_FILENAME = "test_file.txt";
const std::string TEST_FILE_CONTENT = "This is the test file@@@ content??? !!! \n\n";
const size_t TEST_FILESIZE = TEST_FILE_CONTENT.size();

class ConnectionHandlerFixture : public ::testing::Test {
protected:
    ConnectionHandlerFixture() :
        socket_mock(std::make_shared<SocketManagerMock>()),
        callback_mock(std::make_shared<RemoteInteractionCallbackMock>()),
        establisher(socket_mock),
        file_processor(socket_mock)    
    {
        establisher.set_callback(callback_mock);
        file_processor.set_callback(callback_mock);
    }

    std::shared_ptr<SocketManagerMock> socket_mock;
    std::shared_ptr<RemoteInteractionCallbackMock> callback_mock;
    ConnectionEstablisher establisher;
    FileProcessor file_processor;
};

template<typename Type>
net::awaitable<Type> return_immediately(Type t) {
    co_return t;
}
net::awaitable<void> return_immediately() {
    co_return;
}

TEST_F(ConnectionHandlerFixture, connected_successfully) {
    EXPECT_CALL(*socket_mock, establish_connection_async(TEST_PORT))
        .WillOnce(Return(return_immediately()));
    EXPECT_CALL(*socket_mock, get_remote_endpoint())
        .WillRepeatedly(Return(ISocketManager::RemoteEndpoint{TEST_ADDRESS, TEST_PORT}));

    EXPECT_CALL(*callback_mock, connected(TEST_ADDRESS, TEST_PORT));
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
    EXPECT_CALL(*callback_mock, file_transfered());

    net::io_context context;
    net::co_spawn(context, establisher.establish_connection(TEST_PORT), [](std::exception_ptr ptr) {
        if(ptr) std::rethrow_exception(ptr);
    });
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
