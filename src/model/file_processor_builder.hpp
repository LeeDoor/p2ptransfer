#pragma once
#include "file_processor.hpp"
#include "socket_manager.hpp"

namespace p2ptransfer {
namespace model {

/*!
* \brief Builder for \ref FileProcessor .
*
* Builder pattern allows avoiding object initialization at setup stage.
*/
class FileProcessorBuilder {
public:
    using SocketManagerPtr = std::shared_ptr<SocketManager>;

    virtual ~FileProcessorBuilder() = default;

    virtual std::shared_ptr<FileProcessor> create_file_processor(
        NetworkStatusCallback::Callback network_callback,
        ListenerCallback::Callback file_callback,
        SocketManagerPtr socket) = 0;
};

}
}
