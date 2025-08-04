#pragma once
#include "file_processor_builder.hpp"

namespace general {
namespace server {
namespace model {
namespace test {

/// Mock implementation for \ref FileProcessorBuilder .
class FileProcessorMockBuilder : public FileProcessorBuilder {
public:
    FileProcessorMockBuilder (std::shared_ptr<FileProcessor> ptr) :
        ptr_(ptr) {}
    std::shared_ptr<FileProcessor> create_file_processor(
        NetworkStatusCallback::Callback,
        ListenerCallback::Callback,
        SocketManagerPtr) override {
        return ptr_;
    }
private:
    std::shared_ptr<FileProcessor> ptr_;
};

}
}
}
}
