#pragma once

namespace p2ptransfer {
namespace model {

class FileWriterImpl : public FileWriter {
public:
    FileWriterImpl(std::shared_ptr<SocketManager> socket);
    void write_file_to(const Address& address, Port port, const Filename& filename) override;
private:
    std::shared_ptr<SocketManager> socket_;
};

}
}
