#pragma once
#include "file_writer.hpp"

namespace p2ptransfer {

class SocketManager;

namespace model {

class FileWriterImpl : public FileWriter {
public:
    FileWriterImpl(std::shared_ptr<SocketManager> socket);
    net::awaitable<void> write_file(const Filename& filepath) override;

private:
    net::awaitable<void> get_permission();
    net::awaitable<void> send_file();
    void validate_permission(const std::string& permission);
    void calculate_notify_progressbar(size_t bytes_remaining);

    std::shared_ptr<SocketManager> socket_;
    Filename filepath_;
    Filename filename_;
    Filesize filesize_;
};

}
}
