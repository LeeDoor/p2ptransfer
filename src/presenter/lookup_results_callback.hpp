#pragma once
#include "callback.hpp"
#include "address.hpp"
#include "port.hpp"

namespace p2ptransfer {
namespace presenter {

/// The union of \ref ConnectionStatusCallback and \ref FileTransferCallback.
class LookupResultsCallback {
public:
    virtual ~LookupResultsCallback() = default;

    /// Called each time someone responses to lookup request.
    virtual void responce_received(Address address, Port port) = 0;
    virtual void failed_to_lookup(std::string message) = 0;
};

}
using WithLookupResultsCallback = WithCallback<presenter::LookupResultsCallback>;
}
