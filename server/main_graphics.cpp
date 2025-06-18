#include "address_gatherer.hpp"
#include "gui_presenter.hpp"
#include "logger.hpp"
#include "connection_establisher.hpp"

int main(int argc, char** argv) {
    LoggerInitializer init;
    auto nm = std::make_shared<ConnectionEstablisher>();
    auto ag = std::make_shared<AddressGatherer>();
    auto presenter 
        = std::make_shared<GUIPresenter>(argc, argv,
                                         nm, ag); // TODO: dummy replace
    nm->set_callback(presenter);
    ag->set_callback(presenter);
    presenter->setup();
    return presenter->run();
}
