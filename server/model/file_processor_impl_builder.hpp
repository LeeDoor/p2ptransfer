#pragma once
#include "file_processor_builder.hpp"
#include "file_processor_impl.hpp"

class FileProcessorImplBuilder : public FileProcessorBuilder {
public:
    std::shared_ptr<FileProcessor> create_file_processor(std::shared_ptr<FileTransferCallback> callback, 
                                                         std::shared_ptr<SocketManager> socket) override {
        auto fp = std::make_shared<FileProcessorImpl>(socket);
        fp->set_callback(callback);
        return fp;
    }
};
