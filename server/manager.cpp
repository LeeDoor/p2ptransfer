#include "manager.hpp"
#include "argument_parser.hpp"
#include "network_manager.hpp"

int Manager::start(int argc, char** argv) {
    ArgumentData args;
    ArgumentParser parser;
    if(!parser.parse_arguments(argc, argv, args)) {
        return 1;
    }
    listen(args.opened_port);
    return 0;
}
void Manager::listen(Port port) {
    NetworkManager network;
    network.init();
}
