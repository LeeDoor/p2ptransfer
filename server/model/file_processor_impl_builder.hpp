#pragma once
#include "file_processor_builder.hpp"
#include "file_processor_impl.hpp"

namespace general {
namespace server {
namespace model {

class FileProcessorImplBuilder : public FileProcessorBuilder {
public:
    std::shared_ptr<FileProcessor> create_file_processor(FileTransferCallbackPtr callback, 
                                                         SocketManagerPtr socket) override {
        auto fp = std::make_shared<FileProcessorImpl>(socket);
        fp->set_callback(callback);
        return fp;
    }
};

}
}
}
