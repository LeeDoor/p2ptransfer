#pragma once

#include "gather_address_callback.hpp"
#include "i_address_gatherer.hpp"
#include "listener.hpp"
#include "remote_interaction_callback.hpp"
#include "view_callback.hpp"
class Presenter :
    public IRemoteInteractionCallback, 
    public IGatherAddressCallback,
    public IViewCallback {
public:
    virtual ~Presenter() = default;
    virtual void setup() = 0;
    virtual int run() = 0;
protected:
    std::shared_ptr<IListener> listener_;
    std::shared_ptr<IAddressGatherer> address_gatherer_;
};
