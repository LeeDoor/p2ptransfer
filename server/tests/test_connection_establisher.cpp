#include "connection_establisher_impl.hpp"
#include "remote_interaction_callback_mock.hpp"
#include "socket_manager_mock.hpp"
#include "gtest_headers.hpp"

class ConnectionEstablisherFixture : public ::testing::Test {
protected:
    ConnectionEstablisherFixture() :
        socket_mock(std::make_shared<SocketManagerMock>()),
        callback_mock(std::make_shared<RemoteInteractionCallbackMock>()),
        establisher(socket_mock) {
        establisher.set_callback(callback_mock);
    }
    
    void run_establish_connection() {
        net::io_context context;
        net::co_spawn(context, establisher.establish_connection(TEST_PORT), [](std::exception_ptr ptr) {
            if(ptr) std::rethrow_exception(ptr);
        });
        context.run();
    }

    std::shared_ptr<SocketManagerMock> socket_mock;
    std::shared_ptr<RemoteInteractionCallbackMock> callback_mock;
    ConnectionEstablisherImpl establisher;
};

TEST_F(ConnectionEstablisherFixture, establish_connection_successfully) {
    EXPECT_CALL(*socket_mock, establish_connection_async(TEST_PORT))
        .WillOnce(Return(return_immediately()));
    EXPECT_CALL(*socket_mock, get_remote_endpoint())
        .WillRepeatedly(Return(SocketManager::RemoteEndpoint{TEST_ADDRESS, TEST_PORT}));

    EXPECT_CALL(*callback_mock, connected(TEST_ADDRESS, TEST_PORT));
    EXPECT_NO_THROW(run_establish_connection());
}

TEST_F(ConnectionEstablisherFixture, establishThrows_shouldCall_cantOpenSocket) {
    EXPECT_CALL(*socket_mock, establish_connection_async(TEST_PORT))
        .WillOnce(
            [] () -> net::awaitable<void> { 
                throw std::runtime_error("imitating socket opening failure"); 
            });

    EXPECT_CALL(*callback_mock, cant_open_socket());
    EXPECT_THROW(run_establish_connection(), std::runtime_error);
}
