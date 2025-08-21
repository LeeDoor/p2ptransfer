#pragma once
#include "file_writer_builder.hpp"

namespace p2ptransfer {
namespace model {
namespace test {

class FileWriterMockBuilder : public FileWriterBuilder {
public:
    FileWriterMockBuilder(std::shared_ptr<FileWriter> fw)
        : file_writer_{fw}
    {}

    MOCK_METHOD(void, create_file_writer_mock, (), ());
    std::shared_ptr<FileWriter> create_file_writer(
        WithNetworkCallback::Callback network_callback,
        std::shared_ptr<SocketManager>) override {
        create_file_writer_mock();
        file_writer_->set_callback(network_callback);
        return file_writer_;
    }
private:
    std::shared_ptr<FileWriter> file_writer_;
};

}
}
}
