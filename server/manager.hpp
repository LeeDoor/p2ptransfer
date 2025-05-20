#pragma once
#include "argument_data.hpp"

class Manager {
public:
    int start(int argc, char** argv);
    ArgumentData parse_arguments(int argc, char** argv);
    void listen(Port port);
};
