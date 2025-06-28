#include "file_processor_mock.hpp"
#include "gtest_headers.hpp"
#include "connection_establisher_mock.hpp"
#include "listener_impl.hpp"
#include "logger.hpp"
#include "model_mock_factory.hpp"
#include "remote_interaction_callback_mock.hpp"
#include "socket_manager_mock.hpp"
#include "thread_wrapper_mock.hpp"

class ListenerFixture : public ::testing::Test {
protected:
    ListenerFixture() :
        factory_(std::make_shared<ModelMockFactory>()),
        connection_establisher_(std::make_shared<ConnectionEstablisherMock>()),
        file_processor_(std::make_shared<FileProcessorMock>()),
        thread_wrapper_(std::make_shared<ThreadWrapperMock>()),
        callback_(std::make_shared<RemoteInteractionCallbackMock>())
    {
        factory_->set_connection_establisher(connection_establisher_);
        factory_->set_file_processor(file_processor_);
        factory_->set_thread_wrapper(thread_wrapper_);
        factory_->set_socket_manager(std::make_shared<SocketManagerMock>());
        listener_ = std::make_shared<ListenerImpl>(factory_);
        listener_->set_callback(callback_);
    }
    
    std::shared_ptr<ModelMockFactory> factory_;
    std::shared_ptr<ConnectionEstablisherMock> connection_establisher_;
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
    EXPECT_CALL(*thread_wrapper_, execute(::testing::_))
        .WillOnce([](std::function<void()>&& func) {
            func();
        });
    EXPECT_CALL(*connection_establisher_, establish_connection(TEST_PORT))
        .WillOnce(Return(return_immediately()));
    EXPECT_CALL(*file_processor_, try_read_file())
        .WillOnce(Return(return_immediately()));

    listener_->listen_if_not_already(TEST_PORT);
}

TEST_F(ListenerFixture, connectionEstablisherThrewException_HandleWithoutRethrow) {
    EXPECT_CALL(*thread_wrapper_, is_running())
        .WillOnce(Return(false));
    EXPECT_CALL(*thread_wrapper_, execute(::testing::_))
        .Times(testing::AtMost(1))
        .WillOnce([](std::function<void()>&& func) {
            func();
        });
    EXPECT_CALL(*connection_establisher_, establish_connection(TEST_PORT))
        .WillOnce([]() -> net::awaitable<void> {
            throw std::runtime_error("immitating connection problem");
        });

    EXPECT_NO_THROW(listener_->listen_if_not_already(TEST_PORT));
}

TEST_F(ListenerFixture, FileProcessorThrew_HandleWithoutRethrow) {
    EXPECT_CALL(*thread_wrapper_, is_running())
        .WillOnce(Return(false));
    EXPECT_CALL(*thread_wrapper_, execute(::testing::_))
        .WillOnce([](std::function<void()>&& func) {
            func();
        });
    EXPECT_CALL(*connection_establisher_, establish_connection(TEST_PORT))
        .WillOnce(Return(return_immediately()));
    EXPECT_CALL(*file_processor_, try_read_file())
        .WillOnce([]() -> net::awaitable<void> {
            throw std::runtime_error("immitating filing problem");
        });

    EXPECT_NO_THROW(listener_->listen_if_not_already(TEST_PORT));
}
