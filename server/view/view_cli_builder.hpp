#pragma once
#include "view_builder.hpp"

class ViewCLIBuilder : public ViewBuilder {
public:
    std::shared_ptr<View> create_view() override;
};
