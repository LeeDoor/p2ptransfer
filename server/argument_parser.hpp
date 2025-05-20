#pragma once
#include "argument_data.hpp"
#include <optional>

class ArgumentParser {
public:
    bool parse_arguments(int argc, char** argv, ArgumentData& args);
private:
    bool is_valid(int argc, char** argv);
    std::optional<Port> parse_port(char* port_str);
};
