#include "address_gatherer.hpp"
#include "presenter.hpp"

AddressGatherer::~AddressGatherer() {
    gather_thread_.join();
}
void AddressGatherer::set_presenter(std::shared_ptr<Presenter> presenter) {
    presenter_ = presenter;
}
void AddressGatherer::gather_local_address() {
    using udp = net::ip::udp;
    gather_thread_ = std::thread([this] {
        const udp::endpoint ep (net::ip::make_address("192.168.0.1"), 8080);
        udp::socket socket(context_, udp::endpoint(udp::v4(), 8081));
        socket.connect(ep);
        if(auto presenter = presenter_.lock()) {
            presenter->set_address(socket.local_endpoint().address().to_string());
        }
    });
}

