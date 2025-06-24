#pragma once

#include "view.hpp"
class ViewFactory {
public:
    virtual std::shared_ptr<IView> create_view() = 0;
};
