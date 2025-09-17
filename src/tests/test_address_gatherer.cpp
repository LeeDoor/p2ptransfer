#include "address_gatherer_impl.hpp"
#include "address_gatherer_callback_mock.hpp"
#include "socket_manager_mock.hpp"
#include "socket_manager_mock_builder.hpp"
#include "thread_wrapper_mock.hpp"

namespace p2ptransfer {
namespace test {

class AddressGathererFixture : public ::testing::Test {
protected:
    AddressGathererFixture() :
        socket_manager_(std::make_shared<SocketManagerMock>()),
        socket_builder_(std::make_shared<SocketManagerMockBuilder>(socket_manager_)),
        thread_wrapper_(std::make_shared<ThreadWrapperMock>()),
        callback_(std::make_shared<AddressGathererCallbackMock>())
    {
        address_gatherer_ = std::make_shared<AddressGathererImpl>(
            ContextWrapper{},
            thread_wrapper_,
            socket_builder_
        );
        address_gatherer_->set_callback(callback_);
    }

    void check_thread_execute() {
        EXPECT_CALL(*thread_wrapper_, is_running())
            .WillOnce(Return(false));
        EXPECT_CALL(*thread_wrapper_, mock_execute());
    }
    void check_socket_creation() {
        EXPECT_CALL(*socket_builder_, mock_udp_connecting_to(::testing::_, ::testing::_));
    }
    void immitate_endpoint_getter() {
        EXPECT_CALL(*socket_manager_, get_local_endpoint())
            .WillOnce(Return(SocketManager::Endpoint{TEST_LOCADDR, TEST_PORT}));
    }
    void check_success_callback() {
        EXPECT_CALL(*callback_, set_address(TEST_LOCADDR));
    }
    void check_fail_callback() {
        EXPECT_CALL(*callback_, set_address(::testing::_));
    }
    std::shared_ptr<SocketManagerMock> socket_manager_;
    std::shared_ptr<SocketManagerMockBuilder> socket_builder_;
    std::shared_ptr<ThreadWrapperMock> thread_wrapper_;
    std::shared_ptr<AddressGathererImpl> address_gatherer_;
    std::shared_ptr<AddressGathererCallbackMock> callback_;
};

TEST_F(AddressGathererFixture, gatheringAddress_success) {
    check_thread_execute();
    check_socket_creation();
    immitate_endpoint_getter();
    check_success_callback();

    address_gatherer_->gather_local_address();
}

TEST_F(AddressGathererFixture, gatheringTwice_shouldThrow) {
    EXPECT_CALL(*thread_wrapper_, is_running())
        .WillOnce(Return(true));

    EXPECT_THROW(address_gatherer_->gather_local_address(), std::logic_error);
}

TEST_F(AddressGathererFixture, socketCreationFailed_Throw) {
    check_thread_execute();
    EXPECT_CALL(*socket_builder_, mock_udp_connecting_to(::testing::_, ::testing::_))
        .WillOnce(::testing::WithoutArgs([] {
            throw std::runtime_error("immitating connection issue");
        }));
    check_fail_callback();

    address_gatherer_->gather_local_address();
}

}
}
