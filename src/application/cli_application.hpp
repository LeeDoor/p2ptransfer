#pragma once
#include "application.hpp"
#include "general_view_cli.hpp"
#include "listener_view_cli.hpp"
#include "model_builder.hpp"
#include "thread_wrapper_impl.hpp"
#include "transferer_view_cli.hpp"

namespace p2ptransfer {

class CLIApplication {
public:
    CLIApplication(int argc, char** argv) :
        cli_application_{std::make_shared<view::GeneralViewCLI>(argc, argv)},
        application_{
            [this] { return cli_application_; },
            [this] { cli_application_->stop(); }
        }
    {}

    int run() { return application_.run(); }

private:
    std::shared_ptr<view::GeneralViewCLI> cli_application_;
    Application<
        model::ModelBuilder<ThreadWrapperImpl>, 
        view::GeneralViewCLI, 
        view::ListenerViewCLI, 
        view::TransfererViewCLI> application_;
};

}
