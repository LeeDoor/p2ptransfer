#include "connection_establisher.hpp"
#include "remote_interaction_callback_mock.hpp"
#include "socket_manager_mock.hpp"
#include "gtest_headers.hpp"

class ConnectionEstablisherFixture : public ::testing::Test {
protected:
    ConnectionEstablisherFixture() :
        socket_mock(std::make_shared<SocketManagerMock>()),
        callback_mock(std::make_shared<RemoteInteractionCallbackMock>()),
        establisher(socket_mock)
    {
        establisher.set_callback(callback_mock);
    }

    std::shared_ptr<SocketManagerMock> socket_mock;
    std::shared_ptr<RemoteInteractionCallbackMock> callback_mock;
    ConnectionEstablisher establisher;
};

TEST_F(ConnectionEstablisherFixture, connected) {
    EXPECT_CALL(*socket_mock, establish_connection_async(TEST_PORT))
        .WillOnce(Return(return_immediately()));
    EXPECT_CALL(*socket_mock, get_remote_endpoint())
        .WillRepeatedly(Return(ISocketManager::RemoteEndpoint{TEST_ADDRESS, TEST_PORT}));

    EXPECT_CALL(*callback_mock, connected(TEST_ADDRESS, TEST_PORT));
    net::io_context context;
    net::co_spawn(context, establisher.establish_connection(TEST_PORT), [](std::exception_ptr ptr) {
        if(ptr) std::rethrow_exception(ptr);
    });
    EXPECT_NO_THROW({
        context.run();
    });
}
