#pragma once
#include "file_writer.hpp"

namespace p2ptransfer {

class SocketManager;

namespace model {

class FileWriterImpl : public FileWriter {
public:
    FileWriterImpl(std::shared_ptr<SocketManager> socket);
    net::awaitable<void> write_file(const Filename& filename) override;
private:
    net::awaitable<void> get_permission(const Filename& filename, Filesize filesize);
    net::awaitable<void> send_file(const Filename& filename, Filesize filesize);
    void validate_permission(const std::string& filename, const std::string& permission);
    void calculate_notify_progressbar(size_t bytes_remaining, Filesize filesize);

    std::shared_ptr<SocketManager> socket_;
};

}
}
