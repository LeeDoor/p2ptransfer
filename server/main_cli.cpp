#include "logger.hpp"
#include "presenter_impl.hpp"
#include "model_builder.hpp"
#include "signal_handler.hpp"
#include "view_cli.hpp"

int main() {
    LoggerInitializer log_init;
    auto presenter = std::make_shared<PresenterImpl>(
        ModelBuilder::create_listener(),
        ModelBuilder::create_address_gatherer(),
        std::make_shared<ViewCLI>()
    );
    SignalHandler::handle_SIGINT([&presenter]() {
        presenter->stop();
    });
    presenter->setup();
    return presenter->run();
}
