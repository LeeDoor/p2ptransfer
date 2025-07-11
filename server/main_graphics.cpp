#include "logger.hpp"
#include "presenter_impl.hpp"
#include "signal_handler.hpp"
#include "model_builder.hpp"
#include "view_gui.hpp"

int main(int argc, char** argv) {
    LoggerInitializer init;
    auto presenter = std::make_shared<PresenterImpl>(
        ModelBuilder::create_listener(),
        ModelBuilder::create_address_gatherer(),
        std::make_shared<ViewGUI>(std::make_unique<QApplication>(argc, argv))
    );
    SignalHandler::handle_SIGINT([&presenter]() {
        presenter->stop();
    });
    presenter->setup();
    return presenter->run();
}
