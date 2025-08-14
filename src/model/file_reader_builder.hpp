#pragma once
#include "file_reader.hpp"
#include "socket_manager.hpp"

namespace p2ptransfer {
namespace model {

/*!
* \brief Builder for \ref FileReader .
*
* Builder pattern allows avoiding object initialization at setup stage.
*/
class FileReaderBuilder {
public:
    using SocketManagerPtr = std::shared_ptr<SocketManager>;

    virtual ~FileReaderBuilder() = default;

    virtual std::shared_ptr<FileReader> create_file_reader(
        WithNetworkCallback::Callback network_callback,
        ListenerCallback::Callback file_callback,
        SocketManagerPtr socket) = 0;
};

}
}
