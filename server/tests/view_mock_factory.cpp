#include "view_mock_factory.hpp"
#include "view_mock.hpp"

std::shared_ptr<IView> ViewMockFactory::create_view() {
    return std::make_shared<ViewMock>();
}
