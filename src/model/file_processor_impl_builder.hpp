#pragma once
#include "file_processor_builder.hpp"
#include "file_processor_impl.hpp"

namespace p2ptransfer {
namespace model {

/// Implementation for \ref FileProcessorBuilder .
class FileProcessorImplBuilder : public FileProcessorBuilder {
public:
    std::shared_ptr<FileProcessor> create_file_processor(
        NetworkStatusCallback::Callback network_callback,
        ListenerCallback::Callback file_callback,
        SocketManagerPtr socket) override {

        auto fp = std::make_shared<FileProcessorImpl>(socket);
        fp->NetworkStatusCallback::set_callback(network_callback);
        fp->ListenerCallback::set_callback(file_callback);
        return fp;
    }
};

}
}
