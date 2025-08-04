#pragma once
#include "callback.hpp"
#include "listener_callback.hpp"
#include "network_status_callback.hpp"

namespace general {
namespace server {
namespace model {

/*!
* \brief Class for reading remote file.
*
* Reads remote file, when file gathering is agreed with user.
* This class is used by the \ref Listener.
*/
using NetworkCallback = WithCallback<presenter::NetworkStatusCallback>;
using FileVerifyCallback = WithCallback<presenter::ListenerCallback>;
class FileProcessor : public NetworkCallback, public FileVerifyCallback {
public:
    virtual ~FileProcessor() = default;

    /*! \throws std::runtime_error if file was not gathered due
    * to network problems or user's cancellation. */
    virtual net::awaitable<void> try_read_file() = 0;
};

}
}
}
