#include "server_manager.hpp"
#include "argument_parser.hpp"
#include "network_manager.hpp"

int ServerManager::start(int argc, char** argv) {
    ArgumentData args;
    ArgumentParser parser;
    if(!parser.parse_arguments(argc, argv, args)) {
        return 1;
    }
    listen(args.opened_port);
    return 0;
}
void ServerManager::listen(Port port) {
    NetworkManager network;
    network.init(port);
}
