#include "file_processor.hpp"
#include "request_deserializer.hpp"
#include "request_serializer.hpp"

net::awaitable<void> FileProcessor::read_remote_file() {
    auto send_request = co_await handle_send_request();
    co_await send_permission(send_request);
    ask_file_confirmation(send_request);
    std::ofstream output_file = open_file_for_writing(send_request.filename);
    co_await handle_file(output_file , send_request);
}

net::awaitable<SendRequest> FileProcessor::handle_send_request() {
    std::string buffer;
    size_t bytes;
    auto dynamic_buffer = net::dynamic_buffer(buffer, MAX_SEND_REQUEST_SIZE);
    bytes = 
        co_await net::async_read_until(*socket_, dynamic_buffer, 
                                       REQUEST_COMPLETION, 
                                       net::use_awaitable);
    std::string request = buffer.substr(0, bytes);
    RequestDeserializer deserializer;
    auto send_request = deserializer.deserialize_send_request(request);
    co_return send_request;
}

net::awaitable<void> FileProcessor::send_permission(const SendRequest& send_request) {
    auto send_permission = RequestSerializer::serialize_send_permission(send_request.filename);
    ErrorCode ec;
    size_t bytes;
    std::tie(ec, bytes) =
        co_await net::async_write(*socket_, 
                                  net::buffer(*send_permission), 
                                  net::as_tuple(net::use_awaitable));
    if(ec) {
        throw std::runtime_error("failed to write send permission");
    }
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
    constexpr size_t buffer_size = 4096;
    size_t bytes; ErrorCode ec;
    size_t bytes_remaining = send_request.filesize;
    std::array<char, buffer_size> buffer;
    while (bytes_remaining) {
        std::tie(ec, bytes) 
            = co_await net::async_read(*socket_, 
                                       net::buffer(buffer, std::min(buffer_size, bytes_remaining)),
                                       net::as_tuple(net::use_awaitable));
        bytes_remaining -= bytes;
        if(ec && bytes_remaining) {
            throw std::runtime_error("failed to read file: " + ec.what());
        }
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
