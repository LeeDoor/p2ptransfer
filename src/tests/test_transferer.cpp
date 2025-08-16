#include "transferer_impl.hpp"
#include "network_status_callback_mock.hpp"

namespace p2ptransfer {
namespace model {
namespace test {

using namespace presenter::test;

class TransfererFixture : public ::testing::Test {
protected:
    TransfererFixture() :
        callback_{std::make_shared<NetworkStatusCallbackMock>()},
        transferer_{std::make_shared<TransfererImpl>()}
    {
        transferer_->set_callback(callback_);
    }

    std::shared_ptr<NetworkStatusCallbackMock> callback_;
    std::shared_ptr<TransfererImpl> transferer_;
};

TEST_F(TransfererFixture, success) {
}

}
}
}
