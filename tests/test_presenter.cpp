#include "server/presenter_builder.hpp"
#include "view_mock.hpp"
#include "network_manager_mock.hpp"
#include "address_gatherer_mock.hpp"
#include "server/presenter.hpp"
using testing::Return;
class PresenterTest : public testing::Test {
protected:
    PresenterTest() {
        network_mock = std::make_shared<NetworkManagerMock>();
        address_mock = std::make_shared<AddressGathererMock>();
        view_mock = std::make_shared<ViewMock>();
        PresenterBuilder presenter_builder;
        presenter = presenter_builder.Build(view_mock, network_mock, address_mock);
        EXPECT_CALL(*address_mock, gather_local_address())
            .Times(1);
        EXPECT_CALL(*view_mock, run())
            .Times(1);
        presenter->run();
    }
    std::shared_ptr<NetworkManagerMock> network_mock;
    std::shared_ptr<AddressGathererMock> address_mock;
    std::shared_ptr<ViewMock> view_mock;
    std::shared_ptr<Presenter> presenter;

    Address TEST_ADDRESS = "ADDRESS_1584671853";
    Port TEST_PORT = 9654;
};

TEST_F (PresenterTest, AddressGathered) {
    EXPECT_CALL(*view_mock, set_address("Address"));
    presenter->set_address("Address");
}
TEST_F (PresenterTest, ListenCalledWithPort) {
    EXPECT_CALL(*network_mock, init(TEST_PORT));
    EXPECT_CALL(*view_mock, get_port())
        .WillOnce(Return(TEST_PORT));
    presenter->listen();
}
TEST_F(PresenterTest, ConnectionMethods) {
    // abort before opening
    EXPECT_CALL(*view_mock, connection_aborted(TEST_ADDRESS, TEST_PORT));
    presenter->connection_aborted(TEST_ADDRESS, TEST_PORT);

    EXPECT_CALL(*view_mock, connection_opened(TEST_ADDRESS, TEST_PORT));
    presenter->connection_opened(TEST_ADDRESS, TEST_PORT);
    EXPECT_CALL(*view_mock, connected(TEST_ADDRESS, TEST_PORT));
    presenter->connected(TEST_ADDRESS, TEST_PORT);
    EXPECT_CALL(*view_mock, connection_aborted(TEST_ADDRESS, TEST_PORT));
    presenter->connection_aborted(TEST_ADDRESS, TEST_PORT);
    EXPECT_CALL(*view_mock, file_transfered());
    presenter->file_transfered();
    EXPECT_CALL(*view_mock, cant_open_socket());
    presenter->cant_open_socket();

    // abort_twice
    EXPECT_CALL(*view_mock, connection_aborted(TEST_ADDRESS, TEST_PORT));
    presenter->connection_aborted(TEST_ADDRESS, TEST_PORT);
}
