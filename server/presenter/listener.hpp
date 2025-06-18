#pragma once

#include "callback.hpp"
#include "port.hpp"
#include "remote_interaction_callback.hpp"
class IListener : public WithCallback<IRemoteInteractionCallback>{
public:
    virtual ~IListener() = default;
    virtual void listen(Port port) = 0;
};
