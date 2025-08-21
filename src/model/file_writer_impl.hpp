#pragma once
#include "file_writer.hpp"
#include "socket_manager.hpp"

namespace p2ptransfer {
namespace model {

class FileWriterImpl : public FileWriter {
public:
    FileWriterImpl(std::shared_ptr<SocketManager> socket);
    void write_file(const Filename& filename) override;
private:
    std::shared_ptr<SocketManager> socket_;
};

}
}
