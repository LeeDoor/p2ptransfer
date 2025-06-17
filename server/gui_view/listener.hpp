#pragma once

#include "port.hpp"
class IListener {
public:
    virtual ~IListener() = default;
    virtual void listen(Port port) = 0;
};
