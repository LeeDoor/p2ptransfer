#include "argument_parser.hpp"
#include "logger.hpp"

bool ArgumentParser::parse_arguments(int argc, char** argv, ArgumentData& data) {
    if(!is_valid(argc)) {
        return false;
    }
    data.address = argv[1];
    if(auto port = parse_port(argv[2])) {
        data.port = *port;
    } else return false;
    if(auto message = parse_message(argv[3])) {
        data.message = std::move(*message);
    } else return false;
    return true;
}

bool ArgumentParser::is_valid(int argc) {
    if(argc != 4) {
        Logger::log() << "Usage: p2ptransfer <ADDRESS> <PORT> <MESSAGE>" << std::endl;
        return false;
    }   return true;
}
std::optional<Port> ArgumentParser::parse_port(char* port_str) {
    try {
        int iport = std::stoi(port_str);
        if(iport < 0)
            throw std::out_of_range("Port must be positive integer.");
        return static_cast<Port>(iport);
    } catch(std::exception& ex) {
        Logger::log() << "Wrong <PORT> value: " << ex.what() << std::endl;
    }
    return std::nullopt;
}
std::optional<std::string> ArgumentParser::parse_message(char* message) {
    return message;
}
