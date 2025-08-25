#include "cli_args_parser.hpp"
#include "logger.hpp"

namespace p2ptransfer {
namespace view {

CLIArgsParser::CLIArgsParser(int argc, char** argv) 
    : argc{argc}, argv{argv} {}
CLIArgsParser::CLIRunArgs CLIArgsParser::parse_cli_args() {
    try {
        iterate_args();
    } catch(const std::exception& ex) {
        Logger::log() << ex.what() << std::endl;
        print_help();
    }
}

void CLIArgsParser::iterate_args() {
    for(int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--host") == 0) {
            char* next = peek_next_for(i);
        }
    }
}

char* CLIArgsParser::peek_next_for(int i) {
    if(i + 1 == argc) 
        throw std::runtime_error(std::string("should have extra parameter after ") + std::string(argv[i]));
    return argv[i + 1];
}

}
}
