#pragma once
#include "application.hpp"
#include "general_view_cli.hpp"
#include "listener_view_cli.hpp"
#include "transferer_view_cli.hpp"

namespace p2ptransfer {

class CLIApplication {
public:
    CLIApplication(int argc, char** argv) :
        application_{[=] { return view::GeneralViewCLI{argc, argv}; }}
    {}

    int run() { return application_.run(); }

private:
    Application<view::GeneralViewCLI, view::ListenerViewCLI, view::TransfererViewCLI> application_;
};

}
