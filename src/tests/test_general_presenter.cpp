#include "general_presenter.hpp"
#include "address_gatherer_mock.hpp"
#include "general_view_mock.hpp"

namespace p2ptransfer {
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

    std::shared_ptr<AddressGathererMock> address_gatherer_;
    std::shared_ptr<GeneralViewMock> view_;
    std::shared_ptr<GeneralPresenter> presenter_;
};

TEST_F(GeneralPresenterFixture, setup_InstallsCallbacks) {
    EXPECT_EQ(address_gatherer_->get_callback().lock(), presenter_);
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

    presenter_->connection_established(TEST_LOCADDR, TEST_PORT);
}

TEST_F(GeneralPresenterFixture, connectionFailed_showNotification) {
    EXPECT_CALL(*view_, show_socket_error(TEST_ERROR_TEXT));

    presenter_->cant_open_socket(TEST_ERROR_TEXT);
}

TEST_F(GeneralPresenterFixture, fileTransfered_showNotification) {
    EXPECT_CALL(*view_, show_file_success());
    EXPECT_CALL(*view_, update_progressbar_status(100, 0));

    presenter_->transfer_succeed();
}

TEST_F(GeneralPresenterFixture, connectionAborted_showNotification) {
    EXPECT_CALL(*view_, show_transfer_failed(TEST_LOCADDR, TEST_PORT, TEST_ERROR_TEXT));

    presenter_->transfer_failed(TEST_LOCADDR, TEST_PORT, TEST_ERROR_TEXT);
}
TEST_F(GeneralPresenterFixture, setProgressbar_ProvidesStatus) {
    EXPECT_CALL(*view_, update_progressbar_status(100 - 1, testing::_));

    presenter_->set_progressbar(1, 100);
}

TEST_F(GeneralPresenterFixture, multipleProgressbarUpdates_correctSequence) {
    const int PROGRBAR_CALLS = 20;
    std::vector<int> update_calls; 
    update_calls.reserve(PROGRBAR_CALLS);
    EXPECT_CALL(*view_, update_progressbar_status(testing::_, testing::_))
        .Times(PROGRBAR_CALLS)
        .WillRepeatedly([&](double persent, [[maybe_unused]] double _) {
            update_calls.push_back(std::round(persent));
        })
    ;

    for(int i = 0; i < PROGRBAR_CALLS; ++i) {
        presenter_->set_progressbar(i * (100 / PROGRBAR_CALLS), 100);
    }
    ASSERT_EQ(update_calls.size(), PROGRBAR_CALLS);
    for(int i = 0; i < PROGRBAR_CALLS; ++i) {
        EXPECT_EQ(100 - i * (100 / PROGRBAR_CALLS), update_calls[i]);
    }
}

}
}
}
