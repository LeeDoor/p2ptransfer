#pragma once
#include "file_processor.hpp"
#include "network_status_callback.hpp"
#include "socket_manager.hpp"

namespace general {
namespace server {
namespace model {

/*!
* \brief Builder for \ref FileProcessor .
*
* Builder pattern allows avoiding object initialization at setup stage.
*/
class FileProcessorBuilder {
public:
    using CallbackPtr = std::shared_ptr<presenter::NetworkStatusCallback>;
    using SocketManagerPtr = std::shared_ptr<SocketManager>;

    virtual ~FileProcessorBuilder() = default;

    virtual std::shared_ptr<FileProcessor> create_file_processor(
        CallbackPtr callback,
        SocketManagerPtr socket) = 0;
};

}
}
}
