#pragma once
#include "file_processor_builder.hpp"

namespace p2ptransfer {
namespace model {
namespace test {

/// Mock implementation for \ref FileProcessorBuilder .
class FileProcessorMockBuilder : public FileProcessorBuilder {
public:
    FileProcessorMockBuilder (std::shared_ptr<FileProcessor> ptr) :
        ptr_(ptr) {}
    std::shared_ptr<FileProcessor> create_file_processor(
        NetworkStatusCallback::Callback nsc,
        ListenerCallback::Callback lcc,
        SocketManagerPtr) override {
        ptr_->NetworkStatusCallback::set_callback(nsc);
        ptr_->ListenerCallback::set_callback(lcc);
        return ptr_;
    }
private:
    std::shared_ptr<FileProcessor> ptr_;
};

}
}
}
