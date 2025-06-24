#pragma once

#include "port.hpp"
class IViewCallback {
public:
    virtual void listen(Port port) = 0;
};
