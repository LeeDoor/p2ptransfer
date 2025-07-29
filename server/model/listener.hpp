#pragma once
#include "file_processor.hpp"
#include "port.hpp"

namespace general {
namespace server {
namespace model {

/*!
* \brief Part of Model of MVP.
*
* Performs network interaction with users.
* Starts listening for a connection via TCP protocol.
* For incoming connection, starts file gathering.
* \ref RemoteInteractionCallback used for notifying 
* about connection progress and status.
*/
class Listener : public NetworkCallback, public FileVerifyCallback {
public:
    using NetworkCallback::set_callback;
    using FileVerifyCallback::set_callback;
    virtual ~Listener() = default;
 
    /*! Listens for user connection and starts communication.
    * \throws std::runtime_error if file was not gathered due to network 
    * problems or user cancellation.
    */
    virtual void listen_if_not_already(Port port) = 0;
    /// Prepares object for destruction.
    virtual void stop() = 0;
};

}
}
}
