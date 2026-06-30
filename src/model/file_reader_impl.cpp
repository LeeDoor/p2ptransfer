#include "file_reader_impl.hpp"
#include "request_deserializer.hpp"
#include "request_serializer.hpp"
#include "socket_manager.hpp"

namespace p2ptransfer {
namespace model {

FileReaderImpl::FileReaderImpl(SocketManagerPtr socket_manager) :
    socket_manager_(socket_manager) {}

net::awaitable<void> FileReaderImpl::try_read_file() {
    try {
        remote_endpoint_ = socket_manager_->get_remote_endpoint();
        auto send_request = co_await header_handshake();
        co_await read_file(send_request);
        WithNetworkCallback::callback()->transfer_succeed();
    } catch (const std::exception& ex) {
        WithNetworkCallback::callback()->transfer_failed(remote_endpoint_.address, remote_endpoint_.port);
        throw;
    }
}

net::awaitable<SendRequest> FileReaderImpl::header_handshake() {
    auto send_request = co_await handle_send_request();
    if(!ask_file_confirmation(send_request))
        throw std::runtime_error("User denied file gathering");
    co_await send_permission(send_request);
    co_return send_request;
}

net::awaitable<SendRequest> FileReaderImpl::handle_send_request() {
    std::string request = co_await socket_manager_->read_request();
    auto send_request = RequestDeserializer::deserialize_send_request(request);
    validate_send_request(send_request);
    co_return send_request;
}

void FileReaderImpl::validate_send_request(const SendRequest& send_request) {
    std::filesystem::path path_filename(send_request.filename);
    if (path_filename.has_parent_path())
        throw std::runtime_error("filename should not contain directories: " + send_request.filename);
}

bool FileReaderImpl::ask_file_confirmation(const SendRequest& send_request) {
    return WithListenerCallback::callback()->verify_file(send_request.filename, send_request.filesize);
}

net::awaitable<void> FileReaderImpl::send_permission(const SendRequest& send_request) {
    auto send_permission = RequestSerializer::serialize_send_permission(send_request.filename);
    co_await socket_manager_->write(send_permission);
}

net::awaitable<void> FileReaderImpl::read_file(const SendRequest& send_request) {
    std::string temp_filename = generate_temporary_filename(send_request.filename);
    try {
        std::ofstream output_file = open_file_for_writing(temp_filename);
        co_await handle_file(output_file, send_request.filesize);
    }
    catch(const std::runtime_error& ex) {
        std::error_code ec; 
        // try to remove. Even if failed, 
        // keep handling previous error
        std::filesystem::remove(temp_filename, ec);
        throw;
    }
    rename_file(temp_filename, send_request.filename);
}

std::string FileReaderImpl::generate_temporary_filename(const std::string& initial_filename) {
    return initial_filename + std::to_string(rand()) + ".download";
}

std::ofstream FileReaderImpl::open_file_for_writing(const std::string& new_filename) {
    std::ofstream ofs(new_filename, std::ofstream::binary);
    if(!ofs.is_open()) {
        throw std::runtime_error("failed to open file for writing");
    }
    return ofs;
}

net::awaitable<void> FileReaderImpl::handle_file(std::ofstream& os, size_t filesize) {
    size_t bytes_remaining = filesize;
    SocketManager::BufferType buffer;
    while (bytes_remaining) {
        size_t bytes = co_await socket_manager_->read_part_to(buffer, bytes_remaining);
        os.write(buffer.data(), bytes);
        WithNetworkCallback::callback()->set_progressbar(bytes_remaining, filesize);
    }
} 

void FileReaderImpl::rename_file(const std::string& prev_filename, const std::string& new_filename) {
    std::string modified_filename = new_filename;
    auto [new_actual_name, new_extention_with_dot] = split_filename_to_extention(new_filename);
    for(unsigned i = 1; i < UINT_MAX && std::filesystem::exists(modified_filename); ++i) {
        modified_filename = new_actual_name + "(" + std::to_string(i) + ")" + new_extention_with_dot;
        if(i >= UINT_MAX) {
            throw std::runtime_error("Can't imagine a unique name for file " + new_filename);
        }
    }
    std::filesystem::rename(prev_filename, modified_filename);
}

std::tuple<std::string, std::string> FileReaderImpl::split_filename_to_extention(const std::string& new_filename) {
    auto dot_position = new_filename.rfind('.');
    if (dot_position == std::string::npos)
        return { new_filename, "" };
    auto file_name = new_filename.substr(0, dot_position);
    auto file_extention = new_filename.substr(dot_position);
    return { file_name, file_extention };
}

}
}
