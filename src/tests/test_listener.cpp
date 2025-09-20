#include "file_reader_mock.hpp"
#include "listener_callback_mock.hpp"
#include "listener_impl.hpp"
#include "network_status_callback_mock.hpp"
#include "file_reader_mock_builder.hpp"
#include "socket_manager_mock.hpp"
#include "socket_manager_mock_builder.hpp"
#include "thread_wrapper_mock.hpp"

namespace p2ptransfer {
namespace model {
namespace test {

using namespace ::p2ptransfer::test;

class ListenerFixture : public ::testing::Test {
protected:
    ListenerFixture() :
        socket_manager_(std::make_shared<SocketManagerMock>()),
        socket_builder_(std::make_shared<SocketManagerMockBuilder>(socket_manager_)),
        file_builder_(std::make_shared<FileReaderMock>()),
        thread_wrapper_(std::make_shared<ThreadWrapperMock>()),
        network_callback_(std::make_shared<presenter::test::NetworkStatusCallbackMock>()),
        listener_callback_(std::make_shared<presenter::test::ListenerCallbackMock>())
    {
        listener_ = std::make_shared<ListenerImpl>(
            ContextWrapper{},
            thread_wrapper_,
            socket_builder_,
            std::make_shared<FileReaderMockBuilder>(file_builder_)
        );
        listener_->set_callback(network_callback_);
        listener_->set_callback(listener_callback_);

        expect_remote_endpoint_as_required();
    }

    void expect_remote_endpoint_as_required() {
        EXPECT_CALL(*socket_manager_, get_remote_endpoint())
            .Times(::testing::AtLeast(0))
            .WillRepeatedly(Return(SocketManager::Endpoint{TEST_LOCADDR, TEST_PORT}));
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
        EXPECT_CALL(*file_builder_, try_read_file())
            .WillOnce(Return(return_immediately()));
    }

    void check_connection_success_callback() {
        EXPECT_CALL(*network_callback_, connected(TEST_LOCADDR, TEST_PORT));
    }
    void check_failure_callback() {
        EXPECT_CALL(*network_callback_, cant_open_socket());
    }

    std::shared_ptr<SocketManagerMock> socket_manager_;
    std::shared_ptr<SocketManagerMockBuilder> socket_builder_;
    std::shared_ptr<FileReaderMock> file_builder_;
    std::shared_ptr<ThreadWrapperMock> thread_wrapper_;
    std::shared_ptr<presenter::test::NetworkStatusCallbackMock> network_callback_;
    std::shared_ptr<presenter::test::ListenerCallbackMock> listener_callback_;
    std::shared_ptr<ListenerImpl> listener_;

};

net::awaitable<void> task(bool& flag) {
    Logger::log() << "Detached task executed\n";
    flag = true;
    co_return;
}

TEST_F(ListenerFixture, testing_context_Wrapper) {
    ContextWrapper a;
    auto b = a;
    bool flag = false;
    net::co_spawn(*a, task(flag), net::detached);
    b->run();
    EXPECT_TRUE(flag);
}

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

TEST_F(ListenerFixture, FileReaderThrew_HandleWithoutRethrow) {
    check_thread_wrapper_executing();
    check_socket_creation();
    EXPECT_CALL(*file_builder_, try_read_file())
        .WillOnce([]() -> net::awaitable<void> {
            throw std::runtime_error("immitating filing problem");
        });
    check_connection_success_callback();

    EXPECT_NO_THROW(listener_->listen_if_not_already(TEST_PORT));
}

}
}
}
