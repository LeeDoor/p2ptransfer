#pragma once
#include "general_presenter.hpp"
#include "model_builder.hpp"
#include "signal_handler.hpp"
#include "general_view_cli.hpp"

namespace general {
namespace server {

class CliApplication {
public:
    int run() {
        auto presenter = std::make_shared<presenter::GeneralPresenter>(
            //model::ModelBuilder::create_listener(),
            model::ModelBuilder::create_address_gatherer(),
            std::make_shared<view::GeneralViewCLI>()
        );
        SignalHandler::handle_SIGINT([presenter]() {
            presenter->stop();
        });
        presenter->setup();
        return presenter->run();
    }
};

}
}
