#include "file_writer_impl.hpp"
#include "request_deserializer.hpp"
#include "request_serializer.hpp"
#include "socket_manager.hpp"

namespace p2ptransfer {
namespace model {

FileWriterImpl::FileWriterImpl(std::shared_ptr<SocketManager> socket) 
    : socket_{socket}
{}
net::awaitable<void> FileWriterImpl::write_file(const Filename& filename) {
    if(!std::filesystem::exists(filename) || std::filesystem::is_directory(filename))
        throw std::runtime_error("No such file: " + filename);
    Filesize filesize = std::filesystem::file_size(filename);
    co_await get_permission(filename, filesize);
    co_await send_file(filename, filesize);
}
net::awaitable<void> FileWriterImpl::get_permission(const Filename& filename, Filesize filesize) {
    auto request = RequestSerializer::serialize_send_request(filename, filesize);
    co_await socket_->write(request);
    std::string permission_str = co_await socket_->read_request();
    validate_permission(filename, permission_str);
    co_return;
}

void FileWriterImpl::validate_permission(const std::string& filename, const std::string& permission_str) {
    auto permission = RequestDeserializer::deserialize_send_permission(permission_str);
    if (permission.filename != filename)
        throw std::runtime_error("Corrupted endpoint. Permitted to send wrong file");
}

net::awaitable<void> FileWriterImpl::send_file(const Filename& filename, Filesize filesize) {
    std::ifstream ifs(filename, std::ios::binary);
    size_t bytes_remaining = filesize;
    SocketManager::WriteBufferType buffer;
    do {
        buffer.fill(ifs);
        while (!buffer.completed()) {
            co_await socket_->write_part_from(buffer, bytes_remaining);
            calculate_notify_progressbar(bytes_remaining, filesize);
        }
    } while(bytes_remaining);
}

void FileWriterImpl::calculate_notify_progressbar(size_t bytes_remaining, Filesize filesize) {
    double progress = 100.0 - (bytes_remaining * 100.0 / filesize);
    callback()->set_progressbar(progress);
}

}
}
