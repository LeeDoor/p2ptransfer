#pragma once
#include "network_status_callback.hpp"
#include "listener_callback.hpp"

namespace p2ptransfer {

class SocketManager;

namespace model {

class FileReader;

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
        WithListenerCallback::Callback file_callback,
        SocketManagerPtr socket) = 0;
};

}
}
