#pragma once
#include "listener_callback.hpp"
#include "network_status_callback.hpp"

namespace p2ptransfer {
namespace model {

/*!
* \brief Class for reading remote file.
*
* Reads remote file, when file gathering is agreed with user.
* This class is used by the \ref Listener.
*/
class FileReader : public WithNetworkCallback, public ListenerCallback {
public:
    virtual ~FileReader() = default;

    /*! \throws std::runtime_error if file was not gathered due
    * to network problems or user's cancellation. */
    virtual net::awaitable<void> try_read_file() = 0;
};

}
}
