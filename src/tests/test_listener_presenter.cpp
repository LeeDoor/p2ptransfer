#include "listener_presenter.hpp"
#include "listener_mock.hpp"
#include "listener_view_mock.hpp"
#include "network_status_callback_mock.hpp"

namespace p2ptransfer {
namespace server {
namespace presenter {
namespace test {

class ListenerPresenterFixture : public ::testing::Test {
protected:
    using Listener = model::test::ListenerMock;
    using ListenerPtr = std::shared_ptr<Listener>;
    using View = view::test::ListenerViewMock;
    using ViewPtr = std::shared_ptr<View>;

    ListenerPresenterFixture() :
        network_callback_{std::make_shared<NetworkStatusCallbackMock>()},
        listener_{std::make_shared<Listener>()},
        view_{std::make_shared<View>()},
        presenter_{std::make_shared<ListenerPresenter>(network_callback_, listener_, view_)}
    {
        presenter_->setup();
    }
    ~ListenerPresenterFixture() {
        presenter_->stop();
    }

    std::shared_ptr<NetworkStatusCallbackMock> network_callback_;
    ListenerPtr listener_;
    ViewPtr view_;
    std::shared_ptr<ListenerPresenter> presenter_;
};

TEST_F(ListenerPresenterFixture, runSetup_callbacksConfigured) {
    EXPECT_EQ(listener_->get_callback_listener().lock(), presenter_);
    EXPECT_EQ(listener_->get_callback_network().lock(), network_callback_);
    EXPECT_EQ(view_->get_callback().lock(), presenter_);
}

TEST_F(ListenerPresenterFixture, runStop_invokesMemberStops) {
    EXPECT_CALL(*listener_, stop_impl());
}

TEST_F(ListenerPresenterFixture, fileVerificationAsked_ProvideTrueResponse) {
    EXPECT_CALL(*view_, ask_file_verification("file.txt", 1234))
        .WillOnce(Return(true));

    bool userChoice = presenter_->verify_file("file.txt", 1234);

    EXPECT_EQ(userChoice, true);
}

TEST_F(ListenerPresenterFixture, fileVerificationAsked_ProvideFalseResponse) {
    EXPECT_CALL(*view_, ask_file_verification("file.txt", 1234))
        .WillOnce(Return(false));

    bool userChoice = presenter_->verify_file("file.txt", 1234);

    EXPECT_EQ(userChoice, false);
}
}
}
}
}
