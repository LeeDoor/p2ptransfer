#include "argument_parser.hpp"
#include <limits>
#include <iostream>

bool ArgumentParser::parse_arguments(int argc, char** argv, ArgumentData& args) {
    if(!is_valid(argc)) {
        std::cout << "Usage: p2plisten <PORT>." << std::endl;
        return false;
    }
    if(auto port = parse_port(argv[1])) {
        args.port = *port;
    } else {
        return false;
    }
    return true;
}
bool ArgumentParser::is_valid(int argc) {
    return argc == 2; 
}
std::optional<Port> ArgumentParser::parse_port(char* port_str) {
    try {
        int iport = std::stoi(port_str);
        if(iport < 0)
            throw std::out_of_range("Port must be positive integer.");
        return static_cast<Port>(iport);
    } catch(std::exception& ex) {
        std::cout << "Wrong <PORT> value: " << ex.what() << std::endl;
    }
    return std::nullopt;
}
