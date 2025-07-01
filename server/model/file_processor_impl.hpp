#pragma once
#include "callback.hpp"

#include "file_processor.hpp"
#include "request_structures.hpp"
#include "file_transfer_callback.hpp"
#include "socket_manager.hpp"

class Presenter;
class FileProcessorImpl : public FileProcessor, public WithCallback<FileTransferCallback> {
public:
    FileProcessorImpl(std::shared_ptr<SocketManager> socket_manager);

    net::awaitable<void> try_read_file() override;

private:
    net::awaitable<SendRequest> header_handshake();
    net::awaitable<SendRequest> handle_send_request();
    void validate_send_request(const SendRequest& send_request);
    void validate_filename(const Filename& filename);
    bool ask_file_confirmation(const SendRequest& send_request);
    net::awaitable<void> send_permission(const SendRequest& send_request);
    net::awaitable<void> read_file(const SendRequest& send_request);
    std::ofstream open_file_for_writing(const std::string& initial_filename);
    net::awaitable<void> handle_file(std::ofstream& os, size_t filesize);
    void calculate_notify_progressbar(size_t bytes_remaining, size_t filesize);

    std::shared_ptr<SocketManager> socket_manager_;
};
