#pragma once
#include "view_factory.hpp"

class ViewCLIFactory : public ViewFactory {
public:
    std::shared_ptr<View> create_view() override;
};
