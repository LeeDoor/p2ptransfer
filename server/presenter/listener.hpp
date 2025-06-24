#pragma once

#include "callback.hpp"
#include "port.hpp"
#include "remote_interaction_callback.hpp"
class IListener : public WithCallback<RemoteInteractionCallback>{
public:
    virtual ~IListener() = default;
    virtual void listen_if_not_already(Port port) = 0;
};
