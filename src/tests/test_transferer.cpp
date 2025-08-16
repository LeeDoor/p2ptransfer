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
        transferer_{std::make_shared<TransfererImpl>(
            std::make_shared<net::io_context>(),
            thread_wrapper_,
            socket_builder_
        )},
        callback_{std::make_shared<NetworkStatusCallbackMock>()}
    {
        transferer_->set_callback(callback_);
    }

    std::shared_ptr<ThreadWrapperMock> thread_wrapper_;
    std::shared_ptr<SocketManagerMockBuilder> socket_builder_;
    std::shared_ptr<TransfererImpl> transferer_;
    std::shared_ptr<NetworkStatusCallbackMock> callback_;
};

TEST_F(TransfererFixture, success) {
}

}
}
}
