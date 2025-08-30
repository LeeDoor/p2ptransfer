#include "listener_view_cli.hpp"
#include "filesize_formatter.hpp"

namespace p2ptransfer {
namespace view {

ListenerViewCLI::ListenerViewCLI(std::shared_ptr<GeneralViewCLI> view) :
    general_view_{view}
{}

void ListenerViewCLI::setup() {
    general_view_->subscribe_listen(
        [self = shared_from_this()](Port port){ self->listen(port); });
}
void ListenerViewCLI::listen(Port port) {
    Logger::log() << "Listening at port " << port << std::endl;
    callback()->listen(port);
}

bool ListenerViewCLI::ask_file_verification(const Filename& filename, Filesize filesize) {
    std::cout << "Do you want to download file \"" << filename << "\" "
        "(" << FilesizeFormatter::to_string(filesize) << ")? [y/n]: " << std::endl;
    std::string input;
    std::cin >> input;
    return 
        std::strncmp(input.c_str(), "y", 1) == 0 ||
        std::strncmp(input.c_str(), "Y", 1) == 0;
}

}
}
