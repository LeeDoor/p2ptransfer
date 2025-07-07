#include "view_cli.hpp"
#include "filesize_formatter.hpp"

int ViewCLI::run() {
    while(is_running_);
    return 0;
}
void ViewCLI::stop() {
    std::cout << "bye, world!" << std::endl;
    is_running_ = false;
}
void ViewCLI::show_address(const Address& address) {
    std::cout << "Your LAN Address: " << address << std::endl;
    read_port_and_listen();
}
void ViewCLI::read_port_and_listen() {
    std::cout << 
        "Ready to listen for connections\n"
        "Enter your port: ";
    Port port;
    std::cin >> port;
    if(!std::cin.good() || port > 65535) is_running_ = false;
    else callback()->listen(port);
}
void ViewCLI::show_connected(const Address& address, Port port) {
    std::cout << "Connected to remote endpoint: " << address << ":" << port << std::endl;
}
void ViewCLI::show_socket_error() {
    std::cout << "Cant open socket" << std::endl;
}
void ViewCLI::update_progressbar_status(double persent) {
    std::cout << "progress: " << persent << std::endl;
}
bool ViewCLI::ask_file_verification(const Filename& filename, Filesize filesize) {
    std::cout << "Do you want to download file \"" << filename 
        << "\" (" << FilesizeFormatter::to_string(filesize) << ")? [y/n]: " << std::endl;
    char input = '\0';
    do {
        std::cin >> input;
    } while (input != 'y' || input != 'n' || input != 'Y' || input != 'N');
    return input == 'y' || input == 'Y';
}
void ViewCLI::show_file_success() {
    std::cout << "File successfully downloaded" << std::endl;
}
void ViewCLI::show_connection_aborted(const Address& address, Port port) {
    std::cout << "FATAL: Connection aborted with remote endpoint " << address << ":" << port << std::endl;
}
