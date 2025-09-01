#include "file_writer_impl.hpp"
#include "request_deserializer.hpp"
#include "request_serializer.hpp"
#include "socket_manager.hpp"

namespace p2ptransfer {
namespace model {

FileWriterImpl::FileWriterImpl(std::shared_ptr<SocketManager> socket) 
    : socket_{socket}
{}
net::awaitable<void> FileWriterImpl::write_file(const Filename& filepath) {
    filepath_ = filepath;
    if(!std::filesystem::exists(filepath_) || std::filesystem::is_directory(filepath_))
        throw std::runtime_error("No such file: " + filepath_);
    filename_ = std::filesystem::path(filepath_).filename();
    filesize_ = std::filesystem::file_size(filepath_);
    co_await get_permission();
    co_await send_file();
}
net::awaitable<void> FileWriterImpl::get_permission() {
    auto request = RequestSerializer::serialize_send_request(filename_, filesize_);
    co_await socket_->write(request);
    std::string permission_str = co_await socket_->read_request();
    validate_permission(permission_str);
    co_return;
}

void FileWriterImpl::validate_permission(const std::string& permission_str) {
    auto permission = RequestDeserializer::deserialize_send_permission(permission_str);
    if (permission.filename != filename_)
        throw std::runtime_error("Remote user has corrupted client. Permitted to send wrong file");
}

net::awaitable<void> FileWriterImpl::send_file() {
    std::ifstream ifs(filepath_, std::ios::binary);
    size_t bytes_remaining = filesize_;
    SocketManager::WriteBufferType buffer;
    do {
        buffer.fill(ifs);
        while (!buffer.completed()) {
            co_await socket_->write_part_from(buffer, bytes_remaining);
            calculate_notify_progressbar(bytes_remaining);
        }
    } while(bytes_remaining);
    calculate_notify_progressbar(bytes_remaining);
}

void FileWriterImpl::calculate_notify_progressbar(size_t bytes_remaining) {
    double progress = 100.0 - (bytes_remaining * 100.0 / filesize_);
    callback()->set_progressbar(progress);
}

}
}
