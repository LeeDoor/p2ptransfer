#pragma once

#include "file_processor_builder.hpp"
class FileProcessorMockBuilder : public FileProcessorBuilder {
public:
    FileProcessorMockBuilder (std::shared_ptr<FileProcessor> ptr) :
        ptr_(ptr) {}
    std::shared_ptr<FileProcessor> create_file_processor(
        std::shared_ptr<FileTransferCallback>,
        std::shared_ptr<SocketManager>) override {
        return ptr_;
    }
private:
    std::shared_ptr<FileProcessor> ptr_;
};
