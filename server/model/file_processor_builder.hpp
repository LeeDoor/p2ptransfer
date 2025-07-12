#pragma once

#include "file_processor.hpp"
#include "file_transfer_callback.hpp"
#include "socket_manager.hpp"
class FileProcessorBuilder {
public:
    virtual std::shared_ptr<FileProcessor> create_file_processor(std::shared_ptr<FileTransferCallback> callback,
                                                                 std::shared_ptr<SocketManager> socket) = 0;
    virtual ~FileProcessorBuilder() = default;
};
