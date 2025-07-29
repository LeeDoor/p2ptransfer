#pragma once
#include "callback.hpp"
#include "port.hpp"
#include "general_presenter_callback.hpp"

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
class Listener : public WithCallback<presenter::GeneralPresenterCallback>{
public:
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
