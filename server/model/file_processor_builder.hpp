#pragma once
#include "file_processor.hpp"
#include "file_transfer_callback.hpp"
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
    using FileTransferCallbackPtr = std::shared_ptr<general::presenter::FileTransferCallback>;
    using SocketManagerPtr = std::shared_ptr<SocketManager>;

    virtual ~FileProcessorBuilder() = default;

    virtual std::shared_ptr<FileProcessor> create_file_processor(
        FileTransferCallbackPtr callback,
        SocketManagerPtr socket) = 0;
};

}
}
}
