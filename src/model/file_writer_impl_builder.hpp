#pragma once
#include "file_writer_builder.hpp"
#include "file_writer_impl.hpp"

namespace p2ptransfer {
namespace model {

/// Implementation for \ref FileReaderBuilder .
class FileWriterImplBuilder : public FileWriterBuilder {
public:
    std::shared_ptr<FileWriter> create_file_writer(
        WithNetworkCallback::Callback network_callback,
        std::shared_ptr<SocketManager> socket) override {

        auto fp = std::make_shared<FileWriterImpl>(socket);
        fp->WithNetworkCallback::set_callback(network_callback);
        return fp;
    }
};

}
}
