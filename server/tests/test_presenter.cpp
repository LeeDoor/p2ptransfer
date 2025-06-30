#include "gtest_headers.hpp"
#include "listener_mock.hpp"
#include "address_gatherer_mock.hpp"
#include "presenter_impl.hpp"
#include "view_mock.hpp"
#include "model_mock_factory.hpp"
#include "view_mock_factory.hpp"

class PresenterFixture : public ::testing::Test {
protected:
    PresenterFixture():
        listener_(std::make_shared<ListenerMock>()),
        address_gatherer_(std::make_shared<AddressGathererMock>()),
        view_(std::make_shared<ViewMock>())
    {
        auto model_factory = std::make_shared<ModelMockFactory>();
        model_factory->set_listener(listener_);
        model_factory->set_address_gatherer(address_gatherer_);
        presenter_ = std::make_shared<PresenterImpl>(
            model_factory,
            std::make_shared<ViewMockFactory>(view_)
        );
    }

    void check_callbacks_installed() {
        EXPECT_EQ(address_gatherer_->get_callback().lock(), presenter_);
        EXPECT_EQ(listener_->get_callback().lock(), presenter_);
        EXPECT_EQ(view_->get_callback().lock(), presenter_);
    }

    std::shared_ptr<ListenerMock> listener_;
    std::shared_ptr<AddressGathererMock> address_gatherer_;
    std::shared_ptr<ViewMock> view_;
    std::shared_ptr<PresenterImpl> presenter_;
};


TEST_F(PresenterFixture, setup_InstallsCallbacks) {
    presenter_->setup();

    check_callbacks_installed();
}

TEST_F(PresenterFixture, stop_sendsStopCalls) {
    EXPECT_CALL(*listener_, stop());
    EXPECT_CALL(*address_gatherer_, stop());
    EXPECT_CALL(*view_, stop());

    presenter_->stop();
}

TEST_F(PresenterFixture, runWithoutSetup_ThrowsException) {
    EXPECT_THROW(presenter_->run(), std::runtime_error);
}

TEST_F(PresenterFixture, setupAndRun_Success) {
    EXPECT_CALL(*address_gatherer_, gather_local_address());
    EXPECT_CALL(*view_, run());
    
    presenter_->setup();
    presenter_->run();
}

TEST_F(PresenterFixture, listenPressed_calledListener) {
    EXPECT_CALL(*listener_, listen_if_not_already(TEST_PORT));

    presenter_->listen(TEST_PORT);
}

TEST_F(PresenterFixture, addressGathered_calledViewSetter) {
    EXPECT_CALL(*view_, show_address(TEST_ADDRESS));

    presenter_->set_address(TEST_ADDRESS);
}

TEST_F(PresenterFixture, connectionEstablished_showNotification) {
    EXPECT_CALL(*view_, show_connected(TEST_ADDRESS, TEST_PORT));

    presenter_->connected(TEST_ADDRESS, TEST_PORT);
}

TEST_F(PresenterFixture, connectionFailed_showNotification) {
    EXPECT_CALL(*view_, show_socket_error());

    presenter_->cant_open_socket();
}

TEST_F(PresenterFixture, fileTransfered_showNotification) {
    EXPECT_CALL(*view_, show_file_success());

    presenter_->file_transfered();
}

TEST_F(PresenterFixture, connectionAborted_showNotification) {
    EXPECT_CALL(*view_, show_connection_aborted(TEST_ADDRESS, TEST_PORT));

    presenter_->connection_aborted(TEST_ADDRESS, TEST_PORT);
}

TEST_F(PresenterFixture, fileVerificationAsked_ProvideTrueResponse) {
    EXPECT_CALL(*view_, ask_file_verification("file.txt", 1234))
        .WillOnce(Return(true));

    bool userChoice = presenter_->verify_file("file.txt", 1234);

    EXPECT_EQ(userChoice, true);
}

TEST_F(PresenterFixture, fileVerificationAsked_ProvideFalseResponse) {
    EXPECT_CALL(*view_, ask_file_verification("file.txt", 1234))
        .WillOnce(Return(false));

    bool userChoice = presenter_->verify_file("file.txt", 1234);

    EXPECT_EQ(userChoice, false);
}

TEST_F(PresenterFixture, setProgressbar_ProvidesStatus) {
    EXPECT_CALL(*view_, update_progressbar_status(12.4));

    presenter_->set_progressbar(12.4);
}

TEST_F(PresenterFixture, multipleProgressbarUpdates_correctSequence) {
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
