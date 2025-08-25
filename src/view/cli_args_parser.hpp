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
        bool success = false;
        Action action;
        Address address;
        Port port;
        Filename filename;
    };

    CLIArgsParser(int argc, char** argv);

    CLIRunArgs parse_cli_args();
private:
    void iterate_args();
    char* peek_next_for(int i);

    int argc;
    char** argv;
};

}
}
