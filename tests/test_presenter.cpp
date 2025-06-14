#include "view_mock.hpp"
#include "network_manager_mock.hpp"
#include "address_gatherer_mock.hpp"

using ::testing::AtLeast;

TEST (Presenter, TransferCallbacks) {
    NetworkManagerMock network_mock;
    AddressGathererMock address_mock;
    ViewMock view_mock;
    EXPECT_CALL(network_mock, init(8080))
        .Times(AtLeast(1));
    network_mock.init(8080);
    network_mock.init(8080);

}
