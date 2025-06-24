#pragma once
#include "view_factory.hpp"

class ViewMockFactory : public ViewFactory {
public:
    std::shared_ptr<IView> create_view() override;
};
