#include "client_presenter.hpp"
#include "view_mock.hpp"

namespace p2ptransfer {
namespace client {
namespace presenter {
namespace test {

class PresenterFixture : public ::testing::Test {
protected:
    using PresenterPtr = std::shared_ptr<ServerPresenter>;
    using ViewMockPtr = std::shared_ptr<view::test::ViewMock>;
    
    PresenterFixture() :
        view_{std::make_shared<view::test::ViewMock>()},
        presenter_{std::make_shared<ServerPresenter>(view_)}
    {}

    ViewMockPtr view_;
    PresenterPtr presenter_;
};

TEST_F(PresenterFixture, deadCallbacks_beforeSetup) {
    EXPECT_TRUE(view_->get_callback().expired());
    EXPECT_EQ(view_->get_callback().lock(), nullptr); 
}

TEST_F(PresenterFixture, setupCalled_callbacksSetup) {
    presenter_->setup();

    EXPECT_EQ(view_->get_callback().lock(), presenter_);
}

TEST_F(PresenterFixture, runWithoutSetup_exceptionThrown) {
    EXPECT_THROW(presenter_->run(), std::logic_error);
}

}
}
}
}
