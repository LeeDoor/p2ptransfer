#pragma once

#include "callback.hpp"
#include "gather_address_callback.hpp"

namespace general {

/*!
* \brief class for gathering LAN address.
*
* Gathers LAN address using SocketManager.
* Uses \ref WithCallback to notify users about gathering finish.
*/
class AddressGatherer : public WithCallback<GatherAddressCallback> {
public:
    virtual ~AddressGatherer() = default;

    /// Initializes address gathering. 
    /*! \throws std::logic_error if gather_local_address called before previous run is finished. */
    virtual void gather_local_address() = 0;
    /// Prepares an object for being destroyed.
    virtual void stop() = 0;
};

}
