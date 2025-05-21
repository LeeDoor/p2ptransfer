#include "client_manager.hpp"
#include "argument_parser.hpp"
#include <iostream>

int ClientManager::start(int argc, char** argv) {
    ArgumentParser parser;
    ArgumentData data;
    if(!parser.parse_arguments(argc, argv, data)) {
        return 1;
    }
    connect(data);
    return 0;
}
void ClientManager::connect(ArgumentData args) {
    std::cout << "connecting to port " << args.port << " sending " << args.message << std::endl;
}
