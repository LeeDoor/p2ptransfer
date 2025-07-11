#include "view_mock_builder.hpp"

std::shared_ptr<View> ViewMockBuilder::create_view() {
    return view_;
}
