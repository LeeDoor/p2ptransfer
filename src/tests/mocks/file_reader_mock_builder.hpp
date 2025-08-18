#pragma once
#include "file_reader_builder.hpp"

namespace p2ptransfer {
namespace model {
namespace test {

/// Mock implementation for \ref FileReaderBuilder .
class FileReaderMockBuilder : public FileReaderBuilder {
public:
    FileReaderMockBuilder (std::shared_ptr<FileReader> ptr) :
        ptr_(ptr) {}
    std::shared_ptr<FileReader> create_file_reader(
        WithNetworkCallback::Callback nsc,
        ListenerCallback::Callback lcc,
        SocketManagerPtr) override {
        ptr_->WithNetworkCallback::set_callback(nsc);
        ptr_->ListenerCallback::set_callback(lcc);
        return ptr_;
    }
private:
    std::shared_ptr<FileReader> ptr_;
};

}
}
}
