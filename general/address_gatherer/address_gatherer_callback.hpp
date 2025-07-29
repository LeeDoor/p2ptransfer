#pragma once

#include "address.hpp"
namespace general {

/*!
* \brief Callback interface for \ref AddressGatherer users.
*
* Requires \ref AddressGatherer users to be notifiable with gathered address.
*/
class AddressGathererCallback {
public:
    virtual ~AddressGathererCallback() = default;

    /// Called when AddressGatherer finished it's job and gathered address.
    virtual void set_address(const Address& address) = 0;
};

}
