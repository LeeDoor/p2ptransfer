#pragma once
#include "callback.hpp"
#include "common_types.hpp"
#include "request_structures.hpp"
#include "file_transfer_callback.hpp"
#include "socket_manager.hpp"

class Presenter;
class FileProcessor : public WithCallback<FileTransferCallback> {
public:
    FileProcessor(std::shared_ptr<SocketManager> socket_manager_) :
        socket_manager_(socket_manager_){}

    net::awaitable<void> try_read_file();

private:
    net::awaitable<void> read_file();
    net::awaitable<SendRequest> handle_send_request();
    bool ask_file_confirmation(const SendRequest& send_request);
    std::ofstream open_file_for_writing(const std::string& initial_filename);
    net::awaitable<void> send_permission(const SendRequest& send_request);
    net::awaitable<void> handle_file(std::ofstream& os, const SendRequest& send_request);
    void calculate_notify_progressbar(size_t bytes_remaining, size_t filesize);

    std::shared_ptr<SocketManager> socket_manager_;
};
