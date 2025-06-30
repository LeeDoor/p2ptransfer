#include "file_processor_mock.hpp"
#include "gtest_headers.hpp"
#include "listener_impl.hpp"
#include "logger.hpp"
#include "model_mock_factory.hpp"
#include "remote_interaction_callback_mock.hpp"
#include "socket_manager_mock_factory.hpp"
#include "thread_wrapper_mock.hpp"

class ListenerFixture : public ::testing::Test {
protected:
    ListenerFixture() :
        factory_(std::make_shared<ModelMockFactory>()),
        socket_builder_(std::make_shared<SocketManagerMockFactory>()),
        socket_manager_(std::make_shared<SocketManagerMock>()),
        file_processor_(std::make_shared<FileProcessorMock>()),
        thread_wrapper_(std::make_shared<ThreadWrapperMock>()),
        callback_(std::make_shared<RemoteInteractionCallbackMock>())
    {
        factory_->set_file_processor(file_processor_);
        factory_->set_thread_wrapper(thread_wrapper_);
        factory_->set_socket_builder(socket_builder_);
        listener_ = std::make_shared<ListenerImpl>(factory_);
        listener_->set_callback(callback_);

        EXPECT_CALL(*socket_manager_, get_remote_endpoint())
            .Times(::testing::AtLeast(0))
            .WillRepeatedly(Return(SocketManager::Endpoint{TEST_ADDRESS, TEST_PORT}));
    }

    std::shared_ptr<ModelMockFactory> factory_;
    std::shared_ptr<SocketManagerMockFactory> socket_builder_;
    std::shared_ptr<SocketManagerMock> socket_manager_;
    std::shared_ptr<FileProcessorMock> file_processor_;
    std::shared_ptr<ThreadWrapperMock> thread_wrapper_;
    std::shared_ptr<RemoteInteractionCallbackMock> callback_;
    std::shared_ptr<ListenerImpl> listener_;

    LoggerInitializer log_init;
};

TEST_F(ListenerFixture, ifListeningAlready_doNothing) {
    EXPECT_CALL(*thread_wrapper_, is_running())
        .WillOnce(Return(true));

    listener_->listen_if_not_already(TEST_PORT);
}

TEST_F(ListenerFixture, ifNotListening_connectAndReadFile) {
    EXPECT_CALL(*thread_wrapper_, is_running())
        .WillOnce(Return(false));
    EXPECT_CALL(*thread_wrapper_, mock_execute());
    EXPECT_CALL(*socket_builder_, tcp_listening_at(TEST_PORT))
        .WillOnce([this]()->net::awaitable<std::shared_ptr<SocketManager>> {
            co_return static_pointer_cast<SocketManager>(socket_manager_);
        });
    EXPECT_CALL(*file_processor_, try_read_file())
        .WillOnce(Return(return_immediately()));
    EXPECT_CALL(*callback_, connected(TEST_ADDRESS, TEST_PORT));

    listener_->listen_if_not_already(TEST_PORT);
}

TEST_F(ListenerFixture, connectingAttemptThrewException_HandleWithoutRethrow) {
    EXPECT_CALL(*thread_wrapper_, is_running())
        .WillOnce(Return(false));
    EXPECT_CALL(*thread_wrapper_, mock_execute())
        .Times(testing::AtMost(1));
    EXPECT_CALL(*socket_builder_, tcp_listening_at(TEST_PORT))
        .WillOnce([]() -> net::awaitable<std::shared_ptr<SocketManager>> {
            throw std::runtime_error("immitating connection problem");
        });
    EXPECT_CALL(*callback_, cant_open_socket());

    EXPECT_NO_THROW(listener_->listen_if_not_already(TEST_PORT));
}

TEST_F(ListenerFixture, FileProcessorThrew_HandleWithoutRethrow) {
    EXPECT_CALL(*thread_wrapper_, is_running())
        .WillOnce(Return(false));
    EXPECT_CALL(*thread_wrapper_, mock_execute());
    EXPECT_CALL(*socket_builder_, tcp_listening_at(TEST_PORT))
        .WillOnce(Return(return_immediately(static_pointer_cast<SocketManager>(socket_manager_))));
    EXPECT_CALL(*file_processor_, try_read_file())
        .WillOnce([]() -> net::awaitable<void> {
            throw std::runtime_error("immitating filing problem");
        });
    EXPECT_CALL(*callback_, connected(TEST_ADDRESS, TEST_PORT));

    EXPECT_NO_THROW(listener_->listen_if_not_already(TEST_PORT));
}
