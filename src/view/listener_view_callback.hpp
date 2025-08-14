#pragma once
#include "port.hpp"

namespace p2ptransfer {
namespace view {

/// \brief Interface for getting information about User Input.
class ListenerViewCallback {
public:
    virtual ~ListenerViewCallback() = default;

    /// Called if user pressed Listen button. 
    /// \param port should be asked from user before invoking.
    virtual void listen(Port port) = 0;
};

}
}
