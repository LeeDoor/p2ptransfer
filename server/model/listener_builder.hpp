#pragma once
#include "listener.hpp"

class ListenerBuilder {
public:
    virtual std::shared_ptr<Listener> create_listener() = 0; 
};
