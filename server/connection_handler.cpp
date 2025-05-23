#include <boost/asio/co_spawn.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/as_tuple.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/read_until.hpp>
#include "connection_handler.hpp"
#include "network_headers.hpp"
#include "logger.hpp"
#include "request_header_names.hpp"

net::awaitable<void> ConnectionHandler::handle() {
#define HANDLE_RETURN socket_.shutdown(tcpip::socket::shutdown_both); co_return
    std::string data;
    data.reserve(100);
    auto send_request = co_await handle_send_request(data);
    if(!send_request) {
        Logger::log() << "failed to parse send request. shutting down." << std::endl;
        HANDLE_RETURN;
    }
    Logger::log() 
        << "ready for gathering file " << send_request->filename 
        << " of size " << send_request->filesize << std::endl;
    // co_await send_permission({.filename=send_request->filename});
    HANDLE_RETURN;
    co_return;
}

net::awaitable<std::optional<SendRequest>> ConnectionHandler::handle_send_request(std::string& buffer) {
    auto method = co_await handle_method(buffer);
    if(!method) { co_return std::nullopt; }
    if(strcmp(method->c_str(), REQUEST_HEADER.data()) != 0) {
        Logger::log() << "method is wrong: " << *method << " (should be " << REQUEST_HEADER << ")" << std::endl
            << "comparison result: " << method->size() << " " << REQUEST_HEADER_SIZE << std::endl;
        co_return std::nullopt;
    } 
    SendRequest send_request;
    auto filename = co_await handle_filename(buffer);
    if(!filename) co_return std::nullopt;
    auto filesize = co_await handle_filesize(buffer);
    if(!filesize) co_return std::nullopt;
    send_request.filename = std::move(*filename);
    send_request.filesize = *filesize;
    co_return send_request;
}
net::awaitable<std::optional<std::string>> ConnectionHandler::async_readline(std::string& buffer) {
    size_t bytes;
    boost::system::error_code ec;
    std::tie(ec, bytes) = 
        co_await net::async_read_until(socket_, net::dynamic_buffer(buffer), 
                                       '\n', net::as_tuple(net::use_awaitable));
    if(ec) {
        Logger::log() << "failed to read line: " << ec.what() << std::endl;
        co_return std::nullopt;
    }
    std::string result = buffer.substr(0, bytes);
    buffer.erase(0, bytes);
    co_return result;
}
net::awaitable<std::optional<std::string>> ConnectionHandler::handle_method(std::string& buffer) {
    std::optional<std::string> line;
    if(line = co_await async_readline(buffer); !line) co_return std::nullopt;
    std::stringstream sstr(*line);
    std::string word;
    sstr >> word;
    if(!sstr.good() || word.empty()) {
        Logger::log() << "failed to read request header:" << word << std::endl;
        co_return std::nullopt;
    }
    co_return word;
}
net::awaitable<std::optional<std::string>> ConnectionHandler::handle_filename(std::string& buffer) {
    std::optional<std::string> line;
    if(line = co_await async_readline(buffer); !line) co_return std::nullopt;
    std::stringstream sstr(*line);
    std::string header, filename;
    sstr >> header >> filename;
    if(!sstr.good() || 
        header.size() != FILE_HEADER_SIZE ||
        strcmp(header.c_str(), FILE_HEADER.data()) != 0 ||
        filename.empty()) {
        Logger::log() << "failed while reading filename." << std::endl;
        co_return std::nullopt;
    }
    co_return filename;
}
net::awaitable<std::optional<size_t>> ConnectionHandler::handle_filesize(std::string& buffer) {
    std::optional<std::string> line;
    if(line = co_await async_readline(buffer); !line) co_return std::nullopt;
    std::stringstream sstr(*line);
    std::string header;
    size_t filesize;
    sstr >> header >> filesize;
    if(!sstr.good() ||
        header.size() != SIZE_HEADER_SIZE ||
        strcmp(header.c_str(), SIZE_HEADER.data()) != 0) {
        Logger::log() << "failed while reading filesize." << std::endl;
        co_return std::nullopt;
    }
    co_return filesize;
}
