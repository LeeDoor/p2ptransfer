#include "address_gatherer.hpp"
#include "gui_presenter.hpp"
#include "logger.hpp"
#include "network_manager.hpp"

int main(int argc, char** argv) {
    LoggerInitializer init;
    auto view = std::make_shared<GUIPresenter>(argc, argv);
    auto network = std::make_shared<NetworkManager>();
    auto address_gatherer = std::make_shared<AddressGatherer>();
    view->set_callback(network);
    network->set_callback(view);
    address_gatherer->set_callback(view);
    address_gatherer->gather_local_address();
    return view->run();
}
