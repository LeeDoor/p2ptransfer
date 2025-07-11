#include "address_gatherer_impl_builder.hpp"
#include "logger.hpp"
#include "presenter_impl.hpp"
#include "signal_handler.hpp"
#include "listener_impl_builder.hpp"
#include "view_cli_factory.hpp"

int main() {
    LoggerInitializer log_init;
    auto presenter = std::make_shared<PresenterImpl>(
        std::make_shared<ListenerImplBuilder>()->create_listener(),
        std::make_shared<AddressGathererImplBuilder>()->create_address_gatherer(),
        std::make_shared<ViewCLIFactory>()->create_view()
    );
    SignalHandler::handle_SIGINT([&presenter]() {
        presenter->stop();
    });
    presenter->setup();
    return presenter->run();
}
