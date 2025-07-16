#pragma once

#include "logger.hpp"
#include "presenter_impl.hpp"
#include "model_builder.hpp"
#include "signal_handler.hpp"
#include "view_gui.hpp"

namespace general {
namespace server {

class GuiApplication {
public:
    int run(int argc, char** argv) {
        auto application = std::make_shared<QApplication>(argc, argv);
        auto presenter = std::make_shared<presenter::PresenterImpl>(
            model::ModelBuilder::create_listener(),
            model::ModelBuilder::create_address_gatherer(),
            std::make_shared<view::ViewGUI>(application)
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
