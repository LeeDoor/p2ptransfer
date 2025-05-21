#include "client_manager.hpp"
#include "argument_parser.hpp"
#include "network_manager.hpp"

int ClientManager::start(int argc, char** argv) {
    ArgumentParser parser;
    ArgumentData data;
    if(!parser.parse_arguments(argc, argv, data)) {
        return 1;
    }
    connect(std::move(data));
    return 0;
}
void ClientManager::connect(ArgumentData args) {
    NetworkManager network;
    network.initialize_connection(args.address, args.port, std::move(args.message));
}
