#include "general_view_cli.hpp"

namespace general {
namespace server {
namespace view {

int GeneralViewCLI::run() {
    notify_listen();
    return 0;
}

void GeneralViewCLI::notify_listen() {
    for(auto& func : listen_subs_) {
        func();
    }   
}

void GeneralViewCLI::stop() {
    std::cout << "bye, world!" << std::endl;
    is_running_ = false;
}
void GeneralViewCLI::show_address(const Address& address) {
    std::cout << "Your LAN Address: " << address << std::endl;
}
void GeneralViewCLI::show_connected(const Address& address, Port port) {
    std::cout << "Connected to remote endpoint: " << address << ":" << port << std::endl;
}
void GeneralViewCLI::show_socket_error() {
    std::cout << "Cant open socket" << std::endl;
}
void GeneralViewCLI::update_progressbar_status(double persent) {
    std::cout << "progress: " << persent << std::endl;
}
void GeneralViewCLI::show_file_success() {
    std::cout << "File successfully downloaded" << std::endl;
}
void GeneralViewCLI::show_connection_aborted(const Address& address, Port port) {
    std::cout << "FATAL: Connection aborted with remote endpoint " << address << ":" << port << std::endl;
}
void GeneralViewCLI::subscribe_listen(std::function<void()> func) {
    listen_subs_.push_back(func);
}


}
}
}
