#pragma once
#include "address.hpp"
#include "port.hpp"
#include "request_structures.hpp"

namespace p2ptransfer {
namespace view {

class CLIArgsParser {
public:
    enum Action { None, Listen, Transfer };
    struct CLIRunArgs {
        bool valid = true;
        Action action = None;
        Address address = "";
        Port port = 0;
        Filename filename = "";
    };

    CLIArgsParser(int argc, char** argv);

    CLIRunArgs parse_cli_args();
private:
    int argc;
    char** argv;
};

}
}
