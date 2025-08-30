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

    CLIRunArgs handle_cli_args() noexcept;

private:
    /*! \throws std::runtime_error in failure case. */
    void parse_cli_args();
    void add_options();
    Action get_action();
    /*! Should have args.action set before using. */
    void validate();
    void validate_transfer();
    void validate_listen();
    void validate_port();
    void validate_address();
    void validate_filename();

    int argc;
    char** argv;
    po::options_description desc;
    po::variables_map vm;
    CLIRunArgs args;
};

}
}
