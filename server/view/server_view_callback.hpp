#pragma once
#include "port.hpp"
#include "view_callback.hpp"

namespace general {
namespace server {
namespace view {

/// \brief Interface for getting information about User Input.
class ServerViewCallback : public general::view::ViewCallback {
public:
    virtual ~ServerViewCallback() = default;

    /// Called if user pressed Listen button. 
    /// \param port should be asked from user before invoking.
    virtual void listen(Port port) = 0;
};

}
}
}
