#pragma once
#include "address.hpp"
#include "callback.hpp"
#include "port.hpp"
#include "view_callback.hpp"

namespace general {
namespace view {

template<typename ViewCallbackT>
concept ViewCallbackType = std::derived_from<ViewCallbackT, ViewCallback>;

/*!
* \brief View part of MVP.
*
* Provides an interface to show up connection status on the screen.
* The \ref ViewCallback Callback is implemented to allow presenter
* get user input.
*/
template<ViewCallbackType Callback>
class View : public WithCallback<Callback> {
public:
    /// Runs application cycle. Should be called from \ref presenter::Presenter
    virtual int run() = 0;
    /// Stops the cycle and prepares object for destruction.
    virtual void stop() = 0;

    /// Shows LAN address from \ref AddressGatherer.
    virtual void show_address(const Address& address) = 0;

    /// Shows successful connection status.
    virtual void show_connected(const Address& address, Port port) = 0;
    /// Shows failed connection status.
    virtual void show_socket_error() = 0;

    /// Shows progressbar on the screen.
    /*! \param persent floating point number in [0; 100] segment. */
    virtual void update_progressbar_status(double persent) = 0;
    /// Shows message about successfully finished file gathering.
    virtual void show_file_success() = 0;
    /// Shows error if an error occured while sharing file.
    virtual void show_connection_aborted(const Address& address, Port port) = 0;
};

}
}
