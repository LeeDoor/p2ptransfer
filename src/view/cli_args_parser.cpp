#include "cli_args_parser.hpp"
#include "logger.hpp"
#include <boost/program_options.hpp>

namespace p2ptransfer {
namespace view {

CLIArgsParser::CLIArgsParser(int argc, char** argv) 
    : argc{argc}, argv{argv} {}
CLIArgsParser::CLIRunArgs CLIArgsParser::parse_cli_args() {
    namespace po = boost::program_options;
    CLIRunArgs args{};
    try {
        po::options_description desc(
            "(1) p2ptransfer <address> <port> <filename>\n"
            "(2) p2ptransfer <-h|--host address> <-p|--port port> <-f|--filename filename>\n"
            "(3) p2ptransfer get <port>\n"
            "(4) p2ptransfer get <-p|--port port>\n"
        );
        desc.add_options()
            ("help", "produce help message")
        ;
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
        if(vm.count("help")) {
            Logger::log() << desc << std::endl;
            return {.valid = false};
        }

    } catch(const std::exception& ex) {
        Logger::log() << ex.what() << std::endl;
        args.valid = false;
    }
    return args;
}


}
}
