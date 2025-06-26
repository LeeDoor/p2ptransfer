#include "view_mock_factory.hpp"

std::shared_ptr<View> ViewMockFactory::create_view() {
    return view_;
}
