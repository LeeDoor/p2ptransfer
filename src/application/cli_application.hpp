#pragma once
#include "application.hpp"
#include "general_view_cli.hpp"
#include "listener_view_cli.hpp"

namespace p2ptransfer {
namespace server {

class CLIApplication {
public:
    CLIApplication() :
        application_{[] { return view::GeneralViewCLI{}; }}
    {}

    int run() { return application_.run(); }

private:
    Application<view::GeneralViewCLI, view::ListenerViewCLI> application_;
};

}
}
