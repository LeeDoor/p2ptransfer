#include "general_view_cli.hpp"

namespace p2ptransfer {
namespace view {

GeneralViewCLI::GeneralViewCLI(int argc, char** argv) {
    CLIArgsParser parser(argc, argv);
    args_ = parser.handle_cli_args();
    if(!args_.valid) 
        close_program();
}

int GeneralViewCLI::run() {
    run_action();
    while(is_running_);
    return !args_.valid;
}

void GeneralViewCLI::run_action() {
    switch(args_.action) {
        case CLIArgsParser::Listen:
            notify_listen();
            break;
        case CLIArgsParser::Transfer:
            notify_transfer();
            break;
    }
}

void GeneralViewCLI::stop() {
    is_running_ = false;
}

void GeneralViewCLI::notify_listen() {
    for(auto iter = listen_subs_.begin(); iter != listen_subs_.end(); ++iter) {
        (*iter)(args_.port);
    }   
}
void GeneralViewCLI::notify_transfer() {
    for(auto iter = transfer_subs_.begin(); iter != transfer_subs_.end(); ++iter) {
        (*iter)(args_.address, args_.port, args_.filename);
    }   
}

void GeneralViewCLI::show_address(const Address& address) {
    std::cout << "Your LAN Address: " << address << std::endl;
}
void GeneralViewCLI::show_connected(const Address& address, Port port) {
    std::cout << "Connected to remote endpoint: " << address << ":" << port << std::endl;
}
void GeneralViewCLI::show_socket_error() {
    std::cout << "Cant open socket" << std::endl;
    close_program();
}
void GeneralViewCLI::update_progressbar_status(double persent) {
    std::cout << "progress: " << persent << std::endl;
}
void GeneralViewCLI::show_file_success() {
    std::cout << "File successfully transfered" << std::endl;
    close_program();
}
void GeneralViewCLI::show_connection_aborted(const Address& address, Port port) {
    std::cout << "FATAL: Connection aborted with remote endpoint " << address << ":" << port << std::endl;
    close_program();
}
void GeneralViewCLI::close_program() {
    std::raise(SIGINT);
}
void GeneralViewCLI::subscribe_listen(ListenNotification func) {
    listen_subs_.push_back(std::move(func));
}
void GeneralViewCLI::subscribe_transfer(TransferNotification func) {
    transfer_subs_.push_back(std::move(func));
}


}
}
