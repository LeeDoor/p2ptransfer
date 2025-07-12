#include "file_processor_mock.hpp"
#include "listener_impl.hpp"
#include "logger.hpp"
#include "remote_interaction_callback_mock.hpp"
#include "file_processor_mock_builder.hpp"
#include "socket_manager_mock.hpp"
#include "socket_manager_mock_builder.hpp"
#include "thread_wrapper_mock.hpp"

namespace general {
namespace server {
namespace model {
namespace test {

using namespace socket_manager::test;
using namespace thread_wrapper::test;

class ListenerFixture : public ::testing::Test {
protected:
    ListenerFixture() :
        socket_manager_(std::make_shared<SocketManagerMock>()),
        socket_builder_(std::make_shared<SocketManagerMockBuilder>(socket_manager_)),
        file_processor_(std::make_shared<FileProcessorMock>()),
        thread_wrapper_(std::make_shared<ThreadWrapperMock>()),
        callback_(std::make_shared<RemoteInteractionCallbackMock>())
    {
        listener_ = std::make_shared<ListenerImpl>(
            std::make_shared<net::io_context>(),
            thread_wrapper_,
            socket_builder_,
            std::make_shared<FileProcessorMockBuilder>(file_processor_)
        );
        listener_->set_callback(callback_);

        expect_remote_endpoint_as_required();
    }

    void expect_remote_endpoint_as_required() {
        EXPECT_CALL(*socket_manager_, get_remote_endpoint())
            .Times(::testing::AtLeast(0))
            .WillRepeatedly(Return(socket_manager::SocketManager::Endpoint{TEST_ADDRESS, TEST_PORT}));
    }
    void check_thread_wrapper_executing() {
        EXPECT_CALL(*thread_wrapper_, is_running())
            .WillOnce(Return(false));
        EXPECT_CALL(*thread_wrapper_, mock_execute())
            .Times(::testing::AtLeast(1));
    }
    void check_socket_creation() {
        EXPECT_CALL(*socket_builder_, mock_tcp_listening_at(TEST_PORT));
    }
    void check_file_processing() {
        EXPECT_CALL(*file_processor_, try_read_file())
            .WillOnce(Return(return_immediately()));
    }

    void check_connection_success_callback() {
        EXPECT_CALL(*callback_, connected(TEST_ADDRESS, TEST_PORT));
    }
    void check_failure_callback() {
        EXPECT_CALL(*callback_, cant_open_socket());
    }

    std::shared_ptr<SocketManagerMock> socket_manager_;
    std::shared_ptr<SocketManagerMockBuilder> socket_builder_;
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
    check_thread_wrapper_executing();
    check_socket_creation();
    check_file_processing();
    check_connection_success_callback();

    listener_->listen_if_not_already(TEST_PORT);
}

TEST_F(ListenerFixture, connectingAttemptThrewException_HandleWithoutRethrow) {
    check_thread_wrapper_executing();
    EXPECT_CALL(*socket_builder_, mock_tcp_listening_at(TEST_PORT))
        .WillOnce([]() {
            throw std::runtime_error("immitating connection problem");
        });
    check_failure_callback();

    EXPECT_NO_THROW(listener_->listen_if_not_already(TEST_PORT));
}

TEST_F(ListenerFixture, FileProcessorThrew_HandleWithoutRethrow) {
    check_thread_wrapper_executing();
    check_socket_creation();
    EXPECT_CALL(*file_processor_, try_read_file())
        .WillOnce([]() -> net::awaitable<void> {
            throw std::runtime_error("immitating filing problem");
        });
    check_connection_success_callback();

    EXPECT_NO_THROW(listener_->listen_if_not_already(TEST_PORT));
}

TEST_F(ListenerFixture, stoppingWhileThreadOff_doNothing) {
    EXPECT_CALL(*thread_wrapper_, join());

    listener_->stop();
}

}
}
}
}
