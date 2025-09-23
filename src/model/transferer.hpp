#pragma once
#include "network_status_callback.hpp"
#include "request_structures.hpp"

namespace p2ptransfer {
namespace model {

/*!
* \brief Part of Model of MVP.
*
* Performs network interaction with users.
* Connects to remote users and sends them a file.
*/
class Transferer : public WithNetworkCallback {
public:
    virtual ~Transferer() = default;

    /*! Performs network interaction with users.
    * \throws std::runtime_error if file was not gathered due to network problems or remote user cancellation.
    */
    virtual void transfer_file(const Address& address, Port port, const Filename& filename) = 0;
};

}
}
