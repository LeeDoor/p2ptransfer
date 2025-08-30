#pragma once
#include "address.hpp"
#include "port.hpp"
#include "request_structures.hpp"
#include <boost/program_options.hpp>

namespace p2ptransfer {
namespace view {

namespace po = boost::program_options;
class CLIArgsParser {
public:
    enum Action { Listen, Transfer };
    struct CLIRunArgs {
        bool valid = true;
        Action action = Transfer;
        Address address = "";
        Port port = 0;
        Filename filename = "";
    };

    CLIArgsParser(int argc, char** argv);

    CLIRunArgs parse_cli_args();
private:
    /*! Should have args.action set before using. */
    std::string validate();
    std::string validate_transfer();
    std::string validate_listen();

    int argc;
    char** argv;
    po::options_description desc;
    po::variables_map vm;
    CLIRunArgs args;
};

}
}
