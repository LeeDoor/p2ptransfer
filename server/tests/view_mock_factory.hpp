#pragma once
#include "view_factory.hpp"

class ViewMockFactory : public ViewFactory {
public:
    std::shared_ptr<View> create_view() override;
};
