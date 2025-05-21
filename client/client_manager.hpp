#pragma once
#include "argument_data.hpp"
class ClientManager {
public:
    int start(int argc, char** argv);
private:
    void connect(ArgumentData args);
};
