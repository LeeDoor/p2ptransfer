#pragma once
#include "argument_data.hpp"
#include "stdlib.hpp"

class ArgumentParser {
public:
    bool parse_arguments(int argc, char** argv, ArgumentData& data);
private:
    bool is_valid(int argc);
    std::optional<Port> parse_port(char* port_str);
    std::optional<std::string> parse_filename(char* message);
};
