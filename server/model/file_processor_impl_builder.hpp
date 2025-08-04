#pragma once
#include "file_processor_builder.hpp"
#include "file_processor_impl.hpp"

namespace general {
namespace server {
namespace model {

/// Implementation for \ref FileProcessorBuilder .
class FileProcessorImplBuilder : public FileProcessorBuilder {
public:
    std::shared_ptr<FileProcessor> create_file_processor(
        NetworkCallback::Callback network_callback,
        FileVerifyCallback::Callback file_callback,
        SocketManagerPtr socket) override {

        auto fp = std::make_shared<FileProcessorImpl>(socket);
        fp->NetworkCallback::set_callback(network_callback);
        fp->FileVerifyCallback::set_callback(file_callback);
        return fp;
    }
};

}
}
}
