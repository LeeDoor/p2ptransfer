#pragma once
#include "stoppable.hpp"
#include "address.hpp"
#include "port.hpp"

namespace p2ptransfer {
namespace server {
namespace view {

/*!
* \brief View part of MVP.
*
* Provides an interface to show up connection status on the screen.
* The \ref ViewCallback Callback is implemented to allow presenter
* get user input.
*/
class GeneralView : public Stoppable {
public:
    /// Runs application cycle. Should be called from \ref presenter::Presenter
    virtual int run() = 0;

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
}
