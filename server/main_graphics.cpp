#include "address_gatherer.hpp"
#include "gui_presenter.hpp"
#include "listener_impl.hpp"
#include "logger.hpp"

int main(int argc, char** argv) {
    LoggerInitializer init;
    auto presenter 
        = std::make_shared<GUIPresenter>(argc, argv,
                                         std::make_shared<ListenerImpl>(), 
                                         std::make_shared<AddressGatherer>());
    presenter->setup();
    return presenter->run();
}
