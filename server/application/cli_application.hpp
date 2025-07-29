#pragma once
#include "general_presenter.hpp"
#include "model_builder.hpp"
#include "signal_handler.hpp"
#include "general_view_cli.hpp"
#include "listener_view.hpp"

namespace general {
namespace server {

class CliApplication {
public:
    CliApplication() :
        general_view_{std::make_shared<view::GeneralViewCLI>()},
        listener_view_{std::make_shared<view::ListenerView>()}
    {}

    int run() {
        auto general_presenter = std::make_shared<presenter::GeneralPresenter>(
            model::ModelBuilder::create_listener(),
            model::ModelBuilder::create_address_gatherer(),
            general_view_
        );
        SignalHandler::handle_SIGINT([general_presenter]() {
            general_presenter->stop();
        });
        general_presenter->setup();
        return general_presenter->run();
    }

private:
    std::shared_ptr<view::GeneralViewCLI> general_view_;
    std::shared_ptr<view::ListenerView> listener_view_;
};

}
}
