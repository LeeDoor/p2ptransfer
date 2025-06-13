#include "address_gatherer.hpp"
#include "gui_view.hpp"
#include "network_manager.hpp"
#include "presenter.hpp"
#include "logger.hpp"

int main(int argc, char** argv) {
    LoggerInitializer init;
    auto view_ptr = std::make_shared<GUIView>(argc, argv);
    auto network_manager = std::make_shared<NetworkManager>();
    auto address_gatherer = std::make_shared<AddressGatherer>();
    auto presenter = std::make_shared<Presenter>(std::move(view_ptr),
                                                 std::move(network_manager),
                                                 std::move(address_gatherer));
    return presenter->run();
}
