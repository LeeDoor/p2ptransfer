#include "server/presenter_builder.hpp"
#include "view_mock.hpp"
#include "network_manager_mock.hpp"
#include "address_gatherer_mock.hpp"
#include "server/presenter.hpp"

class PresenterTest : public testing::Test {
protected:
    PresenterTest() {
        network_mock = std::make_shared<NetworkManagerMock>();
        address_mock = std::make_shared<AddressGathererMock>();
        view_mock = std::make_shared<ViewMock>();
        PresenterBuilder presenter_builder;
        presenter = presenter_builder.Build(view_mock, network_mock, address_mock);
    }
    std::shared_ptr<NetworkManagerMock> network_mock;
    std::shared_ptr<AddressGathererMock> address_mock;
    std::shared_ptr<ViewMock> view_mock;
    std::shared_ptr<Presenter> presenter;
};

TEST_F (PresenterTest, RunCallsInits) {
    EXPECT_CALL(*address_mock, gather_local_address())
        .Times(1);
    EXPECT_CALL(*view_mock, run())
        .Times(1);
    presenter->run();
}
