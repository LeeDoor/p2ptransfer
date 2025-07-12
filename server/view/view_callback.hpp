#pragma once

#include "port.hpp"
class ViewCallback {
public:
    virtual void listen(Port port) = 0;
    virtual ~ViewCallback() = default;
};
