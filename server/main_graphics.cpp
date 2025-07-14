#include "logger.hpp"
#include "presenter_impl.hpp"
#include "signal_handler.hpp"
#include "model_builder.hpp"
#include "view_gui.hpp"

int main(int argc, char** argv) {
    using namespace general;
    using namespace server;
    LoggerInitializer init;
    auto application = std::make_shared<QApplication>(argc, argv);
    auto presenter = std::make_shared<presenter::PresenterImpl>(
        model::ModelBuilder::create_listener(),
        model::ModelBuilder::create_address_gatherer(),
        std::make_shared<view::ViewGUI>(application)
    );
    signal_handler::SignalHandler::handle_SIGINT([presenter, application]() {
        presenter->stop();
    });
    presenter->setup();
    return presenter->run();
}
