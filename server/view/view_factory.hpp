#pragma once

#include "view.hpp"
class ViewFactory {
public:
    virtual std::shared_ptr<View> create_view() = 0;
};
