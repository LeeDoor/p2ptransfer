#include "general_presenter.hpp"
#include "address_gatherer_mock.hpp"
#include "general_view_mock.hpp"

namespace p2ptransfer {
namespace server {
namespace presenter {
namespace test {

using namespace view::test;
using namespace ::p2ptransfer::test;

class GeneralPresenterFixture: public ::testing::Test {
protected:
    GeneralPresenterFixture():
        address_gatherer_(std::make_shared<AddressGathererMock>()),
        view_(std::make_shared<GeneralViewMock>())
    {
        presenter_ = std::make_shared<GeneralPresenter>(
            address_gatherer_,
            view_
        );
        presenter_->setup();
    }
    ~GeneralPresenterFixture() {
        presenter_->stop();
    }

    std::shared_ptr<AddressGathererMock> address_gatherer_;
    std::shared_ptr<GeneralViewMock> view_;
    std::shared_ptr<GeneralPresenter> presenter_;
};

TEST_F(GeneralPresenterFixture, setup_InstallsCallbacks) {
    EXPECT_EQ(address_gatherer_->get_callback().lock(), presenter_);
}

TEST_F(GeneralPresenterFixture, stop_sendsStopCalls) {
    EXPECT_CALL(*address_gatherer_, stop_impl());
    EXPECT_CALL(*view_, stop_impl());
}

TEST_F(GeneralPresenterFixture, setupAndRun_Success) {
    EXPECT_CALL(*address_gatherer_, gather_local_address());
    EXPECT_CALL(*view_, run());

    presenter_->run();
}

TEST_F(GeneralPresenterFixture, addressGathered_calledViewSetter) {
    EXPECT_CALL(*view_, show_address(TEST_LOCADDR));

    presenter_->set_address(TEST_LOCADDR);
}

TEST_F(GeneralPresenterFixture, connectionEstablished_showNotification) {
    EXPECT_CALL(*view_, show_connected(TEST_LOCADDR, TEST_PORT));

    presenter_->connected(TEST_LOCADDR, TEST_PORT);
}

TEST_F(GeneralPresenterFixture, connectionFailed_showNotification) {
    EXPECT_CALL(*view_, show_socket_error());

    presenter_->cant_open_socket();
}

TEST_F(GeneralPresenterFixture, fileTransfered_showNotification) {
    EXPECT_CALL(*view_, show_file_success());

    presenter_->file_transfered();
}

TEST_F(GeneralPresenterFixture, connectionAborted_showNotification) {
    EXPECT_CALL(*view_, show_connection_aborted(TEST_LOCADDR, TEST_PORT));

    presenter_->connection_aborted(TEST_LOCADDR, TEST_PORT);
}
TEST_F(GeneralPresenterFixture, setProgressbar_ProvidesStatus) {
    EXPECT_CALL(*view_, update_progressbar_status(12.4));

    presenter_->set_progressbar(12.4);
}

TEST_F(GeneralPresenterFixture, multipleProgressbarUpdates_correctSequence) {
    std::vector<double> update_calls; 
    update_calls.reserve(20);
    EXPECT_CALL(*view_, update_progressbar_status(testing::_))
        .Times(20)
        .WillRepeatedly([&](double persent) {
            update_calls.push_back(persent);
        });

    for(int i = 0; i < 20; ++i) {
        presenter_->set_progressbar(i * 5);
    }

    for(int i = 0; i < 20; ++i) {
        EXPECT_EQ(i * 5, update_calls[i]);
    }
}

}
}
}
}
