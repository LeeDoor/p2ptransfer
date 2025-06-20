#include "file_processor.hpp"
#include "request_deserializer.hpp"
#include "request_serializer.hpp"

net::awaitable<void> FileProcessor::read_remote_file() {
    auto send_request = co_await handle_send_request();
    co_await send_permission(send_request);
    if(!ask_file_confirmation(send_request))
        co_return;
    std::ofstream output_file = open_file_for_writing(send_request.filename);
    co_await handle_file(output_file , send_request);
}

net::awaitable<SendRequest> FileProcessor::handle_send_request() {
    std::string request = co_await socket_manager_->read_request();
    auto send_request = RequestDeserializer::deserialize_send_request(request);
    co_return send_request;
}

net::awaitable<void> FileProcessor::send_permission(const SendRequest& send_request) {
    auto send_permission = RequestSerializer::serialize_send_permission(send_request.filename);
    co_await socket_manager_->send_response(std::move(send_permission));
}
bool FileProcessor::ask_file_confirmation(const SendRequest& send_request) {
    if(auto callback = callback_.lock()) {
        return callback->verify_file(send_request);
    }
    throw std::runtime_error("callback of FileProcessor is dead");
}
std::ofstream FileProcessor::open_file_for_writing(const std::string& initial_filename) {
    std::ofstream ofs("READED_" + initial_filename, std::ios::binary);
    if(!ofs.is_open()) {
        throw std::runtime_error("failed to open file for writing");
    }
    return ofs;
}
net::awaitable<void> FileProcessor::handle_file(std::ofstream& os, const SendRequest& send_request) {
    size_t bytes_remaining = send_request.filesize;
    ISocketManager::BufferType buffer;
    while (bytes_remaining) {
        size_t bytes = co_await socket_manager_->read_file_part_to(buffer, bytes_remaining);
        os.write(buffer.data(), bytes);
        calculate_notify_progressbar(bytes_remaining, send_request.filesize);
    }
} 

void FileProcessor::calculate_notify_progressbar(size_t bytes_remaining, size_t filesize) {
    if(auto callback = callback_.lock()) {
        double progress = 100.0 - (bytes_remaining * 100.0 / filesize);
        callback->set_progressbar(progress);
    }
}
