#pragma once
#include <string>
#include "port.hpp"
struct ArgumentData {
    std::string address;
    Port port;
    std::string filename;
};
