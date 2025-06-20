#pragma once
#include "callback.hpp"
#include "common_types.hpp"
#include "request_structures.hpp"
#include "file_transfer_callback.hpp"
#include "socket_manager.hpp"

class Presenter;
class FileProcessor : public WithCallback<IFileTransferCallback> {
public:
    FileProcessor(std::unique_ptr<ISocketManager> socket_manager_) :
        socket_manager_(std::move(socket_manager_)){}

    net::awaitable<void> read_remote_file();

private:
    net::awaitable<SendRequest> handle_send_request();
    bool ask_file_confirmation(const SendRequest& send_request);
    std::ofstream open_file_for_writing(const std::string& initial_filename);
    net::awaitable<void> send_permission(const SendRequest& send_request);
    net::awaitable<void> handle_file(std::ofstream& os, const SendRequest& send_request);
    void calculate_notify_progressbar(size_t bytes_remaining, size_t filesize);

    std::unique_ptr<ISocketManager> socket_manager_;
};
