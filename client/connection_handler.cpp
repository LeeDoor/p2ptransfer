#include "connection_handler.hpp"
#include "logger.hpp"
#include "request_serializer.hpp"

net::awaitable<int> ConnectionHandler::handle(std::string filepath) {
    std::ifstream ifs(filepath, std::ios::binary);
    if(!ifs.is_open()) {
        Logger::log() << "failed to open file for reading." << std::endl;
        co_return 1;
    }
    namespace fs = std::filesystem;
    size_t filesize = fs::file_size(filepath);
    if(! co_await send_request(fs::path(filepath).filename().string(), filesize)) {
        Logger::log() << "failed to send send_request." << std::endl;
        co_return 2;
    }
    if(! co_await read_permission()) {
        Logger::log() << "failed to read send_permission." << std::endl;
        co_return 3;
    }
    if(! co_await send_file(ifs, filesize)) {
        Logger::log() << "failed to send file." << std::endl;
        co_return 4;
    }
    Logger::log() << "file sent successfully." << std::endl;
    co_return 0;
}
template <typename IStream>
net::awaitable<bool> ConnectionHandler::send_file(IStream& is, size_t filesize) {
    constexpr size_t BUFFER_SIZE = 4096;
    size_t bytes_remaining = filesize;
    size_t bytes; boost::system::error_code ec;
    std::string buff;
    buff.resize(BUFFER_SIZE, '*');
    while(bytes_remaining) {
        size_t chunk_size = std::min(buff.size(), bytes_remaining);
        is.read(buff.data(), chunk_size);
        std::tie(ec, bytes) =
            co_await net::async_write(*socket_, net::buffer(buff, chunk_size),
                                      net::as_tuple(net::use_awaitable));
        bytes_remaining -= bytes;
        if(ec && bytes_remaining) {
            Logger::log() << std::endl << " failed while writing file: " << ec.what() << std::endl;
            co_return false;
        }
        Logger::log() << "\r" << std::flush  << std::setprecision(2) << 
            "progress: " << std::setw(6)<< std::max(0.0, 100.0 - (bytes_remaining * 100.0 / filesize)) << " %";
    }
    Logger::log() << std::endl;
    co_return true;
}
net::awaitable<bool> ConnectionHandler::send_request(const std::string& filename, size_t filesize) {
    auto send_request_opt = 
        RequestSerializer::serialize_send_request(filename, filesize);
    if(!send_request_opt) { 
        Logger::log() << "failed to serialize send_request." << std::endl;
        co_return false;
    }
    std::string send_request(std::move(*send_request_opt));
    boost::system::error_code ec;
    size_t bytes;
    std::tie(ec, bytes) = co_await net::async_write(*socket_, net::buffer(send_request),
                                                  net::as_tuple(net::use_awaitable));
    if(ec) {
        Logger::log() << "failed to write data: " << ec.what() << std::endl;
        co_return false;
    }
    co_return true;
}
net::awaitable<bool> ConnectionHandler::read_permission() {
    boost::system::error_code ec;
    size_t bytes;
    std::string read_buffer;
    std::tie(ec, bytes) = co_await net::async_read_until(*socket_, net::dynamic_buffer(read_buffer),
                                                         "\n\n", net::as_tuple(net::use_awaitable));
    if(ec && ec != net::error::eof) {
        Logger::log() << "failed to read data: " << ec.what() << std::endl;
        co_return false;
    }
    co_return true;
}
