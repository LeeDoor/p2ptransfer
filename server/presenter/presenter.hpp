#pragma once
#include "gather_address_callback.hpp"
#include "remote_interaction_callback.hpp"
#include "view_callback.hpp"

namespace general {
namespace server {
namespace presenter {

/*!
* \brief presenter class of MVP pattern.
*
* Performs communicaiton between \ref view::View and Model's \ref model::Listener and \ref AddressGatherer.
* Implements callback interfaces to communicate with each of them.
*/
class Presenter :
    public model::RemoteInteractionCallback, 
    public GatherAddressCallback,
    public view::ViewCallback {

public:
    /// Should be called after initialization before run.
    virtual void setup() = 0;
    /// Starts infinite execution loop, which can be stoppped with \ref stop() method. 
    /// Should be called after \ref setup(). 
    virtual int run() = 0;
    /// Prepares objects' for stopping. Calling View's and Model's stop() methods.
    virtual void stop() = 0;

    virtual ~Presenter() = default;
};

}
}
}
