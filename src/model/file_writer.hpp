#pragma once
#include "network_status_callback.hpp"
#include "request_structures.hpp"

namespace p2ptransfer {
namespace model {

/*!
* \brief Class for writing file to remote user.
*
* Asks remote user to confirm file sharing and sends file to him.
*/
class FileWriter : public WithNetworkCallback {
public:
    virtual ~FileWriter() = default;

    virtual net::awaitable<void> write_file(const Filename& filepath) = 0;
};

}
}
