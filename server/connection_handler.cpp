#include "connection_handler.hpp"
#include "logger.hpp"
#include "request_deserializer.hpp"
#include "request_serializer.hpp"

net::awaitable<int> ConnectionHandler::handle() {
    std::string data;
    auto send_request = co_await handle_send_request(data);
    if(!send_request) {
        Logger::log() << "failed to perform send request. shutting down." << std::endl;
        co_return 1;
    }
    if(auto callback = callback_.lock()) {
        if(!callback->verify_file(*send_request)) 
            co_return 5;
    }
    if(!co_await send_permission(*send_request)) {
        Logger::log() << "failed to perform send permission. shutting down." << std::endl;
        co_return 2;
    }
    std::ofstream ofs("READED_" + send_request->filename, std::ios::binary);
    if(!ofs.is_open()) {
        Logger::log() << "failed to open file for writing." << std::endl;
        co_return 3;
    }
    if(!co_await handle_file(ofs, *send_request)) {
        Logger::log() << "failed to read file." << std::endl;
        co_return 4;
    }
    Logger::log() << "file written successfully." << std::endl;
    co_return 0;
}

template<typename OStream>
net::awaitable<bool> ConnectionHandler::handle_file(OStream& os, const SendRequest& send_request) {
    constexpr size_t buffer_size = 4096;
    size_t bytes;
    ErrorCode ec;
    size_t bytes_remaining = send_request.filesize;
    std::array<char, buffer_size> buffer;
    Logger::progressbar_init();
    while (bytes_remaining) {
        std::tie(ec, bytes) 
            = co_await net::async_read(*socket_, net::buffer(buffer, std::min(buffer_size, bytes_remaining)),
                                       net::as_tuple(net::use_awaitable));
        bytes_remaining -= bytes;
        if(ec && bytes_remaining) {
            Logger::progressbar_stop();
            Logger::log() << bytes_remaining << " bytes remaining. failed to read file: " << ec.what() << std::endl;
            co_return false;
        }
        os.write(buffer.data(), bytes);
        double progress = 100.0 - (bytes_remaining * 100.0 / send_request.filesize);
        Logger::log_progressbar(progress);
        if(auto callback = callback_.lock()) {
            callback->set_progressbar_status(progress);
        }
    }
    Logger::progressbar_stop();
    co_return true;
} 
net::awaitable<bool> ConnectionHandler::send_permission(const SendRequest& send_request) {
    auto send_permission = RequestSerializer::serialize_send_permission(send_request.filename);
    if(!send_permission) {
        Logger::log() << "failed to serialize send permission." << std::endl;
        co_return false;
    }
    ErrorCode ec;
    size_t bytes;
    std::tie(ec, bytes) =
        co_await net::async_write(*socket_, net::buffer(*send_permission), net::as_tuple(net::use_awaitable));
    if(ec) {
        Logger::log() << "failed to write send permission." << std::endl;
        co_return false;
    }
    co_return true;
}
net::awaitable<std::optional<SendRequest>> ConnectionHandler::handle_send_request(std::string& buffer) {
    size_t bytes;
    ErrorCode ec;
    std::tie(ec, bytes) = 
        co_await net::async_read_until(*socket_, net::dynamic_buffer(buffer, 512), 
                                       "\n\n", net::as_tuple(net::use_awaitable));
    if(ec && ec != boost::asio::error::eof) {
        Logger::log() << "failed to read line: " << ec.what() << std::endl;
        co_return std::nullopt;
    }
    std::string request;
    request = buffer.substr(0, bytes);
    buffer.erase(0, bytes);
    RequestDeserializer deserializer;
    if(auto send_request = deserializer.deserialize_send_request(request)) {
        co_return *send_request;
    }
    co_return std::nullopt;
}
