#pragma once
#include "file_processor.hpp"
#include "request_structures.hpp"
#include "socket_manager.hpp"

namespace general {
namespace server {

namespace presenter { class ServerPresenter; }

namespace model {

/// Implementation for \ref FileProcessor using \ref SocketManager .
class FileProcessorImpl : public FileProcessor {
public:
    using SocketManagerPtr = std::shared_ptr<SocketManager>;
    /*
    * \param socket_manager provided \ref SocketManager pointer 
    * should be already established with remote user.
    */
    FileProcessorImpl(SocketManagerPtr socket_manager);

    net::awaitable<void> try_read_file() override;

private:
    /*!
    * \brief Gathers SendRequest and sends permission if user allowed.
    * \throws std::runtime_error if user denied file gathering or 
    * connection problem occured.
    */
    net::awaitable<SendRequest> header_handshake();
    /// Reads and verifies send request.
    net::awaitable<SendRequest> handle_send_request();
    void validate_send_request(const SendRequest& send_request);
    /// Checks if filename contains directories or forbiddden filename.
    void validate_filename(const Filename& filename);
    bool ask_file_confirmation(const SendRequest& send_request);
    net::awaitable<void> send_permission(const SendRequest& send_request);
    net::awaitable<void> read_file(const SendRequest& send_request);
    std::ofstream open_file_for_writing(const std::string& initial_filename);
    net::awaitable<void> handle_file(std::ofstream& os, size_t filesize);
    /// Calculates the progress of file downloading and calls the callback.
    void calculate_notify_progressbar(size_t bytes_remaining, size_t filesize);

    SocketManagerPtr socket_manager_;
};

}
}
}
