#pragma once

#include "view.hpp"
class ViewBuilder {
public:
    virtual std::shared_ptr<View> create_view() = 0;
};
