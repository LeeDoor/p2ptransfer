#pragma once
#include "callback.hpp"
#include "common_types.hpp"
#include "request_structures.hpp"
#include "file_transfer_callback.hpp"
class Presenter;
class FileProcessor : public WithCallback<IFileTransferCallback> {
public:
    FileProcessor(net::io_context& ctx, SockPtr socket)
    : io_(ctx), socket_(std::move(socket)){}
    net::awaitable<int> read_remote_file();
private:
    constexpr static size_t MAX_SEND_REQUEST_SIZE = 512;
    constexpr static std::string_view REQUEST_COMPLETION = "\n\n";

    net::awaitable<std::optional<SendRequest>> handle_send_request(std::string& buffer);
    template<typename OStream>
    net::awaitable<bool> handle_file(OStream& os, const SendRequest& send_request);
    net::awaitable<bool> send_permission(const SendRequest& send_request);
    net::io_context& io_;
    SockPtr socket_;
};
