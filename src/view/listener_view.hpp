#pragma once
#include "callback.hpp"
#include "stoppable.hpp"
#include "request_structures.hpp"
#include "listener_view_callback.hpp"

namespace p2ptransfer {
namespace view {

/*!
* \brief Listener View part of MVP
*
* Provides the interface for the Listener module to
* communicate with the user.
*/
class ListenerView : public WithCallback<ListenerViewCallback> {
public:
    /// Should be called immediately after initialization.
    virtual void setup() = 0;
    /// Asks user about file gathering confirmation.
    virtual bool ask_file_verification(const Filename& filename, Filesize filesize) = 0;
};

}
}
