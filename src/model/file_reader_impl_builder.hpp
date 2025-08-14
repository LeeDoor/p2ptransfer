#pragma once
#include "file_reader_builder.hpp"
#include "file_reader_impl.hpp"

namespace p2ptransfer {
namespace model {

/// Implementation for \ref FileReaderBuilder .
class FileReaderImplBuilder : public FileReaderBuilder {
public:
    std::shared_ptr<FileReader> create_file_reader(
        WithNetworkCallback::Callback network_callback,
        ListenerCallback::Callback file_callback,
        SocketManagerPtr socket) override {

        auto fp = std::make_shared<FileReaderImpl>(socket);
        fp->WithNetworkCallback::set_callback(network_callback);
        fp->ListenerCallback::set_callback(file_callback);
        return fp;
    }
};

}
}
