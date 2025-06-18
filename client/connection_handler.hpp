#pragma once
#include "common_types.hpp"

class FileProcessor {
public:
    FileProcessor(net::io_context& ctx, SockPtr socket)
        : context_(ctx), socket_(std::move(socket)){}
    net::awaitable<int> handle(std::string filepath);
private:
    template <typename IStream>
    net::awaitable<bool> send_file(IStream& is, size_t filesize);
    net::awaitable<bool> send_request(const std::string& filename, size_t filesize);
    net::awaitable<bool> read_permission();

    net::io_context& context_;
    SockPtr socket_;
};
