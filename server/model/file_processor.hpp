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
    net::awaitable<void> read_remote_file();
private:
    constexpr static size_t MAX_SEND_REQUEST_SIZE = 512;
    constexpr static std::string_view REQUEST_COMPLETION = "\n\n";

    net::awaitable<SendRequest> handle_send_request();
    bool ask_file_confirmation(const SendRequest& send_request);
    std::ofstream open_file_for_writing(const std::string& initial_filename);
    net::awaitable<void> send_permission(const SendRequest& send_request);
    net::awaitable<void> handle_file(std::ofstream& os, const SendRequest& send_request);
    void calculate_notify_progressbar(size_t bytes_remaining, size_t filesize);
    net::io_context& io_;
    SockPtr socket_;
};
