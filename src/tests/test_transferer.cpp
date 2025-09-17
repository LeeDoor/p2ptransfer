#include "file_writer_mock.hpp"
#include "file_writer_mock_builder.hpp"
#include "socket_manager_mock.hpp"
#include "socket_manager_mock_builder.hpp"
#include "transferer_impl.hpp"
#include "network_status_callback_mock.hpp"
#include "thread_wrapper_mock.hpp"

namespace p2ptransfer {
namespace model {
namespace test {

using namespace presenter::test;
using namespace p2ptransfer::test;

class TransfererFixture : public ::testing::Test {
protected:
    TransfererFixture() :
        thread_wrapper_{std::make_shared<ThreadWrapperMock>()},
        socket_manager_{std::make_shared<SocketManagerMock>()},
        socket_builder_{std::make_shared<SocketManagerMockBuilder>(socket_manager_)},
        file_writer_{std::make_shared<FileWriterMock>()},
        file_writer_builder_{std::make_shared<FileWriterMockBuilder>(file_writer_)},
        transferer_{std::make_shared<TransfererImpl>(
            ContextWrapper{},
            thread_wrapper_,
            socket_builder_,
            file_writer_builder_
        )},
        network_callback_{std::make_shared<NetworkStatusCallbackMock>()}
    {
        transferer_->set_callback(network_callback_);
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
        EXPECT_CALL(*socket_builder_, mock_tcp_connecting_to(TEST_LOCADDR, TEST_PORT));
    }
    void check_connection_success_callback() {
        EXPECT_CALL(*network_callback_, connected(TEST_LOCADDR, TEST_PORT));
    }
    void should_not_try_to_get_endpoint() {
        EXPECT_CALL(*socket_manager_, get_remote_endpoint())
            .Times(0);
    }
    void simulate_endpoint_receiver() {
        EXPECT_CALL(*socket_manager_, get_remote_endpoint())
            .Times(::testing::AtLeast(0))
            .WillRepeatedly(Return(SocketManager::Endpoint{TEST_LOCADDR, TEST_PORT}));
    }
    void check_failure_callback() {
        EXPECT_CALL(*network_callback_, cant_open_socket());
    }
    void check_file_create_and_transfer() {
        EXPECT_CALL(*file_writer_builder_, create_file_writer_mock());
        EXPECT_CALL(*file_writer_, write_file).WillOnce(Return(return_immediately()));
        EXPECT_CALL(*network_callback_, file_transfered());
    }
    
    std::shared_ptr<ThreadWrapperMock> thread_wrapper_;
    std::shared_ptr<SocketManagerMock> socket_manager_;
    std::shared_ptr<SocketManagerMockBuilder> socket_builder_;
    std::shared_ptr<FileWriterMock> file_writer_;
    std::shared_ptr<FileWriterMockBuilder> file_writer_builder_;
    std::shared_ptr<TransfererImpl> transferer_;
    std::shared_ptr<NetworkStatusCallbackMock> network_callback_;
};

TEST_F(TransfererFixture, ifAlreadyTransfering_ignore) {
    should_not_try_to_get_endpoint();
    EXPECT_CALL(*thread_wrapper_, is_running())
        .WillOnce(Return(true));
    EXPECT_CALL(*socket_builder_, mock_tcp_connecting_to).Times(0);

    transferer_->transfer_file(TEST_LOCADDR, TEST_PORT, TEST_FILENAME);
}

TEST_F(TransfererFixture, ifNotListening_connectAndTransferFile) {
    check_thread_wrapper_executing();
    check_socket_creation();
    check_connection_success_callback();
    simulate_endpoint_receiver();
    check_file_create_and_transfer();

    transferer_->transfer_file(TEST_LOCADDR, TEST_PORT, TEST_FILENAME);
}

TEST_F(TransfererFixture, connectingAttemptThrewException_HandleWithoutRethrow) {
    check_thread_wrapper_executing();
    should_not_try_to_get_endpoint();
    EXPECT_CALL(*socket_builder_, mock_tcp_connecting_to(TEST_LOCADDR, TEST_PORT))
        .WillOnce([]() {
            throw std::runtime_error("immitating connection problem");
        });
    check_failure_callback();

    EXPECT_NO_THROW(transferer_->transfer_file(TEST_LOCADDR, TEST_PORT, TEST_FILENAME));
}

TEST_F(TransfererFixture, FileWriterThrew_HandleWithoutRethrow) {
    check_thread_wrapper_executing();
    check_socket_creation();
    simulate_endpoint_receiver();
    EXPECT_CALL(*file_writer_builder_, create_file_writer_mock())
        .WillOnce([]() -> net::awaitable<void> {
            throw std::runtime_error("immitating filing problem");
        });
    check_connection_success_callback();
    EXPECT_CALL(*network_callback_, connection_aborted(TEST_LOCADDR, TEST_PORT));

    EXPECT_NO_THROW(transferer_->transfer_file(TEST_LOCADDR, TEST_PORT, TEST_FILENAME));
}
}
}
}
