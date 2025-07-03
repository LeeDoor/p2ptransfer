#include "view_cli.hpp"
#include "filesize_formatter.hpp"

int ViewCLI::run() {
    std::cout << "Hello, world!" << std::endl;
    callback()->listen(45954);
    while(is_running_);
    return 0;
}
void ViewCLI::stop() {
    std::cout << "bye, world!" << std::endl;
    is_running_ = false;
}
void ViewCLI::show_address(const Address& address) {
    std::cout << "Your LAN Address: " << address << std::endl;
}
void ViewCLI::show_connected(const Address& address, Port port) {
    std::cout << "Connected to remote endpoint: " << address << ":" << port << std::endl;
}
void ViewCLI::show_socket_error() {
    std::cout << "Failed to connect to socket" << std::endl;
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
