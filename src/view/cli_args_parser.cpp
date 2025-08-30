#include "cli_args_parser.hpp"
#include "logger.hpp"

namespace p2ptransfer {
namespace view {

CLIArgsParser::CLIArgsParser(int argc, char** argv) 
    : argc{argc}
    , argv{argv}
    , desc {
        "(1) p2ptransfer <address> <port> <filename>\n"
        "(2) p2ptransfer <-h|--host address> <-p|--port port> <-f|--filename filename>\n"
        "(3) p2ptransfer --get <port>\n"
        "(4) p2ptransfer --get <-p|--port port>\n"
    }
    , args{} 
{}
CLIArgsParser::CLIRunArgs CLIArgsParser::parse_cli_args() {
    try {
        desc.add_options()
            ("help", "produce help message")
            ("host,h", po::value<Address>(&args.address), "Address to connect to. Combined with -p <port>")
            ("port,p", po::value<Port>(&args.port), "Port to connect to OR port to listen at")
            ("filename,f", po::value<Filename>(&args.filename), "Directory to a file being sent to remote user")
            ("get", "Indicates the file receiving action")
        ;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
        if(vm.count("help")) {
            Logger::log() << desc << std::endl;
            return {.valid = false};
        }
        if(vm.count("get"))
            args.action = Listen;
        else args.action = Transfer;
        if(auto problem = validate(); !problem.empty()) {
            throw std::runtime_error(problem + " provided.");
        }
    } catch(const std::exception& ex) {
        Logger::log() 
            << ex.what() << std::endl
            << "To see the correct usage guide, use --help" << std::endl;
        args.valid = false;
    }
    return args;
}

std::string CLIArgsParser::validate() {
    switch(args.action) {
        case Transfer:
            return validate_transfer();
        case Listen:
            return validate_listen();
    }
}

std::string CLIArgsParser::validate_transfer() {
    if(!args.port) 
        throw std::runtime_error("No --port provided");
    if(!args.address.empty() || !args.filename.empty()) 
        throw std::runtime_error("Extra arguments provided");
}
std::string CLIArgsParser::validate_listen() {
    if(!args.address.empty())
        if(!args.port)
            if(!args.filename.empty())
}

}
}
