#include "general_view_cli.hpp"

namespace p2ptransfer {
namespace view {

GeneralViewCLI::GeneralViewCLI(int argc, char** argv) {
    
}

int GeneralViewCLI::run() {
    while(is_running_);
    std::raise(SIGINT);
    return 0;
}

void GeneralViewCLI::notify_listen() {
    for(auto iter = listen_subs_.begin(); iter != listen_subs_.end(); ++iter) {
        (*iter)();
    }   
}
void GeneralViewCLI::notify_transfer() {
    for(auto iter = transfer_subs_.begin(); iter != transfer_subs_.end(); ++iter) {
        (*iter)();
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
    is_running_ = false;
}
void GeneralViewCLI::update_progressbar_status(double persent) {
    std::cout << "progress: " << persent << std::endl;
}
void GeneralViewCLI::show_file_success() {
    std::cout << "File successfully downloaded" << std::endl;
    is_running_ = false;
}
void GeneralViewCLI::show_connection_aborted(const Address& address, Port port) {
    std::cout << "FATAL: Connection aborted with remote endpoint " << address << ":" << port << std::endl;
    is_running_ = false;
}
void GeneralViewCLI::subscribe_listen(std::function<void()> func) {
    listen_subs_.push_back(std::move(func));
}
void GeneralViewCLI::subscribe_transfer(std::function<void()> func) {
    transfer_subs_.push_back(std::move(func));
}


}
}
