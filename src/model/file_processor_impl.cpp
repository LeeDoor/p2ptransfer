#include "file_processor_impl.hpp"
#include "request_deserializer.hpp"
#include "request_serializer.hpp"

namespace p2ptransfer {
namespace server {
namespace model {

FileProcessorImpl::FileProcessorImpl(SocketManagerPtr socket_manager) :
    socket_manager_(socket_manager) {}

net::awaitable<void> FileProcessorImpl::try_read_file() {
    try {
        auto send_request = co_await header_handshake();
        co_await read_file(send_request);
        NetworkStatusCallback::callback()->file_transfered();
    } catch (const std::exception& ex) {
        auto remote_endpoint = socket_manager_->get_remote_endpoint();
        NetworkStatusCallback::callback()->connection_aborted(remote_endpoint.address, remote_endpoint.port);
        throw;
    }
}

net::awaitable<SendRequest> FileProcessorImpl::header_handshake() {
    auto send_request = co_await handle_send_request();
    if(!ask_file_confirmation(send_request))
        throw std::runtime_error("User denied file gathering");
    co_await send_permission(send_request);
    co_return send_request;
}

net::awaitable<SendRequest> FileProcessorImpl::handle_send_request() {
    std::string request = co_await socket_manager_->read_request();
    auto send_request = RequestDeserializer::deserialize_send_request(request);
    validate_send_request(send_request);
    co_return send_request;
}

void FileProcessorImpl::validate_send_request(const SendRequest& send_request) {
    validate_filename(send_request.filename);
}

void FileProcessorImpl::validate_filename(const Filename& filename) {
    std::filesystem::path path_filename(filename);
    if (path_filename.has_parent_path())
        throw std::runtime_error("filename should not contain directories: " + filename);
}

bool FileProcessorImpl::ask_file_confirmation(const SendRequest& send_request) {
    return ListenerCallback::callback()->verify_file(send_request.filename, send_request.filesize);
}

net::awaitable<void> FileProcessorImpl::send_permission(const SendRequest& send_request) {
    auto send_permission = RequestSerializer::serialize_send_permission(send_request.filename);
    co_await socket_manager_->write(send_permission);
}

net::awaitable<void> FileProcessorImpl::read_file(const SendRequest& send_request) {
    std::ofstream output_file = open_file_for_writing(send_request.filename);
    co_await handle_file(output_file, send_request.filesize);
}

std::ofstream FileProcessorImpl::open_file_for_writing(const std::string& initial_filename) {
    std::ofstream ofs("READED_" + initial_filename, std::ofstream::binary);
    if(!ofs.is_open()) {
        throw std::runtime_error("failed to open file for writing");
    }
    return ofs;
}

net::awaitable<void> FileProcessorImpl::handle_file(std::ofstream& os, size_t filesize) {
    size_t bytes_remaining = filesize;
    SocketManager::BufferType buffer;
    while (bytes_remaining) {
        size_t bytes = co_await socket_manager_->read_part_to(buffer, bytes_remaining);
        os.write(buffer.data(), bytes);
        calculate_notify_progressbar(bytes_remaining, filesize);
    }
} 

void FileProcessorImpl::calculate_notify_progressbar(size_t bytes_remaining, size_t filesize) {
    if(auto callback = NetworkStatusCallback::callback_.lock()) {
        double progress = 100.0 - (bytes_remaining * 100.0 / filesize);
        callback->set_progressbar(progress);
    }
}

}
}
}
