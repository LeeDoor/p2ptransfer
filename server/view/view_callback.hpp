#pragma once
#include "port.hpp"

namespace general {
namespace server {
namespace view {

/// \brief Interface for getting information about User Input.
class ViewCallback {
public:
    virtual ~ViewCallback() = default;

    /// Called if user pressed Listen button. 
    /// \param port should be asked from user before invoking.
    virtual void listen(Port port) = 0;
};

}
}
}
