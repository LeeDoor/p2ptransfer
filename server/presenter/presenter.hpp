#pragma once
#include "gather_address_callback.hpp"
#include "remote_interaction_callback.hpp"
#include "view_callback.hpp"

namespace general {
namespace server {
namespace presenter {

class Presenter :
    public model::RemoteInteractionCallback, 
    public GatherAddressCallback,
    public view::ViewCallback {

public:
    virtual void setup() = 0;
    virtual int run() = 0;
    virtual void stop() = 0;

    virtual ~Presenter() = default;
};

}
}
}
