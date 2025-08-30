#include "cli_args_parser.hpp"
#include "logger.hpp"

namespace p2ptransfer {
namespace view {

CLIArgsParser::CLIArgsParser(int argc, char** argv) 
    : argc{argc}
    , argv{argv}
    , desc {
        "(1) p2ptransfer <-h|--host address> <-p|--port port> <-f|--filename filename>\n"
        "(2) p2ptransfer --get <-p|--port port>\n"
        "\n"
        "(1) -- sends given <filename> to the remote user using <address>:<port> endpoint\n"
        "(2) -- starts to receive any incoming files at <port>. Should start listening before trying to send\n"
    }
    , args{} 
{}
CLIArgsParser::CLIRunArgs CLIArgsParser::handle_cli_args() noexcept {
    try {
        parse_cli_args();
    } catch(const std::exception& ex) {
        Logger::log() 
            << ex.what() << std::endl
            << "To see the correct usage guide, use --help" << std::endl;
        args.valid = false;
    }
    return args;
}

void CLIArgsParser::parse_cli_args() {
    add_options();
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if(vm.count("help")) {
        Logger::log() << desc << std::endl;
        args.valid = false;
        return;
    }
    args.action = get_action();
    validate();
}

void CLIArgsParser::add_options() {
    desc.add_options()
        ("help", "produce help message")
        ("host,h", po::value<Address>(&args.address), "Address to connect to. Combined with -p <port>")
        ("port,p", po::value<Port>(&args.port), "Port to connect to OR port to listen at")
        ("filename,f", po::value<Filename>(&args.filename), "Directory to a file being sent to remote user")
        ("get", "Indicates the file receiving action")
    ;
}

CLIArgsParser::Action CLIArgsParser::get_action() {
    if(vm.count("get"))
        return Listen;
    return Transfer;
}

void CLIArgsParser::validate() {
    switch(args.action) {
        case Transfer:
            return validate_transfer();
        case Listen:
            return validate_listen();
    }       
}

void CLIArgsParser::validate_transfer() {
    validate_address();
    validate_port();
    validate_filename();
}
void CLIArgsParser::validate_listen() {
    validate_port();
}

void CLIArgsParser::validate_port() {
    if(args.port == 0) 
        throw std::runtime_error("No --port provided");
    if(args.port > 65535)
        throw std::runtime_error("Port value should be in range [1; 65535]");
}
void CLIArgsParser::validate_address() {
    if(args.address.empty()) 
        throw std::runtime_error("No --host provided");
    std::regex ipv4_regex(
        "((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\\.){3}"
        "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])"
    );
    if(!std::regex_match(args.address, ipv4_regex)) {
        throw std::runtime_error("Address should be IPv4");
    }
}
void CLIArgsParser::validate_filename() {
    if(args.filename.empty())
        throw std::runtime_error("No --filename provided");
    if(!std::filesystem::exists(args.filename) || std::filesystem::is_directory(args.filename))
        throw std::runtime_error("No such file: " + args.filename);
}

}
}
