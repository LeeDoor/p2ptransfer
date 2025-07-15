#pragma once
#include "file_processor.hpp"
#include "file_transfer_callback.hpp"
#include "socket_manager.hpp"

namespace general {
namespace server {
namespace model {

class FileProcessorBuilder {
public:
    using FileTransferCallbackPtr = std::shared_ptr<FileTransferCallback>;
    using SocketManagerPtr = std::shared_ptr<SocketManager>;

    virtual std::shared_ptr<FileProcessor> create_file_processor(FileTransferCallbackPtr callback,
                                                                 SocketManagerPtr socket) = 0;
    virtual ~FileProcessorBuilder() = default;
};

}
}
}
