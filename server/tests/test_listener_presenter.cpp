#include "listener_presenter.hpp"
#include "listener_mock.hpp"
#include "listener_view_mock.hpp"
#include "network_status_callback_mock.hpp"

namespace general {
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

TEST_F(ListenerPresenterFixture, success) {
    // EXPECT_EQ(false, true);
}

}
}
}
}
