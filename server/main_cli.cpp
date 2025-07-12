#include "logger.hpp"
#include "presenter_impl.hpp"
#include "model_builder.hpp"
#include "signal_handler.hpp"
#include "view_cli.hpp"

int main() {
    using namespace general;
    LoggerInitializer log_init;
    auto presenter = std::make_shared<presenter::PresenterImpl>(
        model::ModelBuilder::create_listener(),
        model::ModelBuilder::create_address_gatherer(),
        std::make_shared<view::ViewCLI>()
    );
    signal_handler::SignalHandler::handle_SIGINT([presenter]() {
        presenter->stop();
    });
    presenter->setup();
    return presenter->run();
}
