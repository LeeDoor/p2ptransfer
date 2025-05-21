#pragma once
#include "argument_data.hpp"

class ServerManager {
public:
    int start(int argc, char** argv);
private:
    ArgumentData parse_arguments(int argc, char** argv);
    void listen(ArgumentData data);
};
