#pragma once

#include "callback.hpp"
#include "address_gatherer_callback.hpp"
#include "stoppable.hpp"

namespace p2ptransfer {

/*!
* \brief class for gathering LAN address.
*
* Gathers LAN address using SocketManager.
* Uses \ref WithCallback to notify users about gathering finish.
*/
class AddressGatherer : public WithCallback<AddressGathererCallback>, public Stoppable {
public:
    virtual ~AddressGatherer() = default;

    /// Initializes address gathering on separated thread. 
    /*! \throws std::logic_error if gather_local_address called before previous run is finished. */
    virtual void gather_local_address() = 0;
};

}
