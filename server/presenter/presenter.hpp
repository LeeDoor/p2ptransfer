#pragma once

#include "gather_address_callback.hpp"
#include "remote_interaction_callback.hpp"
#include "view_callback.hpp"
class Presenter :
    public RemoteInteractionCallback, 
    public GatherAddressCallback,
    public ViewCallback {
public:
    virtual void setup() = 0;
    virtual int run() = 0;
    virtual void stop() = 0;
    virtual ~Presenter() = default;
};
