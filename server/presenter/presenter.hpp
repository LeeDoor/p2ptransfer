#pragma once

#include "gather_address_callback.hpp"
#include "address_gatherer.hpp"
#include "listener.hpp"
#include "remote_interaction_callback.hpp"
#include "view_callback.hpp"
class Presenter :
    public RemoteInteractionCallback, 
    public GatherAddressCallback,
    public ViewCallback {
public:
    virtual ~Presenter() = default;
    virtual void setup() = 0;
    virtual int run() = 0;
protected:
    std::shared_ptr<Listener> listener_;
    std::shared_ptr<AddressGatherer> address_gatherer_;
};
