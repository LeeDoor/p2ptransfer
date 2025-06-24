#include "gtest_headers.hpp"
#include "listener_mock.hpp"
#include "address_gatherer_mock.hpp"
#include "presenter_impl.hpp"
#include "view_mock.hpp"
#include "model_mock_factory.hpp"
#include "view_mock_factory.hpp"

class PresenterFixture : public ::testing::Test {
protected:
    PresenterFixture() {
        presenter_ = std::make_shared<PresenterImpl>(
            std::make_shared<ModelMockFactory>(),
            std::make_shared<ViewMockFactory>()
        );
    }

    std::shared_ptr<PresenterImpl> presenter_;
    std::shared_ptr<ListenerMock> listener_;
    std::shared_ptr<AddressGathererMock> address_gatherer_;
    std::shared_ptr<ViewMock> view_;
};

TEST_F(PresenterFixture, setupAndRun_success) {
    presenter_->setup();
    presenter_->run();
}
