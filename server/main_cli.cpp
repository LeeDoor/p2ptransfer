#include "logger.hpp"
#include "model_impl_factory.hpp"
#include "presenter_impl.hpp"
#include "view_cli_factory.hpp"
#include "signal_handler.hpp"

int main() {
    LoggerInitializer log_init;
    auto presenter = std::make_shared<PresenterImpl>(
        std::make_shared<ModelImplFactory>(),
        std::make_shared<ViewCLIFactory>()
    );
    SignalHandler::handle_SIGINT([&presenter]() {
        presenter->stop();
    });
    presenter->setup();
    return presenter->run();
}
