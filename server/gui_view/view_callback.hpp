#pragma once

#include "port.hpp"
class IViewCallback {
public:
    virtual ~IViewCallback() = default;
    virtual void listen(Port port) = 0;
};
