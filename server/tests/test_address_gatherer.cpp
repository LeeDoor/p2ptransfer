#include "address_gatherer_impl.hpp"
#include "gather_address_callback_mock.hpp"

#include "model_mock_factory.hpp"
#include "socket_manager_mock.hpp"
#include "socket_manager_mock_factory.hpp"
#include "thread_wrapper_mock.hpp"

class AddressGathererFixture : public ::testing::Test {
protected:
    AddressGathererFixture() :
        factory_(std::make_shared<ModelMockFactory>()),
        socket_manager_(std::make_shared<SocketManagerMock>()),
        socket_builder_(std::make_shared<SocketManagerMockFactory>(socket_manager_)),
        thread_wrapper_(std::make_shared<ThreadWrapperMock>()),
        callback_(std::make_shared<GatherAddressCallbackMock>())
    {
        factory_->set_socket_builder(socket_builder_);
        factory_->set_thread_wrapper(thread_wrapper_);
        address_gatherer_ = std::make_shared<AddressGathererImpl>(factory_);
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
            .WillOnce(Return(SocketManager::Endpoint{TEST_ADDRESS, TEST_PORT}));
    }
    void check_success_callback() {
        EXPECT_CALL(*callback_, set_address(TEST_ADDRESS));
    }
    void check_fail_callback() {
        EXPECT_CALL(*callback_, set_address(::testing::_));
    }
    std::shared_ptr<ModelMockFactory> factory_;
    std::shared_ptr<SocketManagerMock> socket_manager_;
    std::shared_ptr<SocketManagerMockFactory> socket_builder_;
    std::shared_ptr<ThreadWrapperMock> thread_wrapper_;
    std::shared_ptr<AddressGathererImpl> address_gatherer_;
    std::shared_ptr<GatherAddressCallbackMock> callback_;
};

TEST_F(AddressGathererFixture, gatheringAddress_success) {
    check_thread_execute();
    check_socket_creation();
    immitate_endpoint_getter();
    check_success_callback();

    address_gatherer_->gather_local_address();
}

TEST_F(AddressGathererFixture, stopped_joinThread) {
    EXPECT_CALL(*thread_wrapper_, join());

    address_gatherer_->stop();
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
