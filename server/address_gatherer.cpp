#include "address_gatherer.hpp"

AddressGatherer::~AddressGatherer() {
    gather_thread_.join();
}
void AddressGatherer::gather_local_address() {
    using udp = net::ip::udp;
    gather_thread_ = std::thread([this] {
        const udp::endpoint ep (net::ip::make_address("192.168.0.1"), 8080);
        udp::socket socket(context_, udp::endpoint(udp::v4(), 8081));
        ErrorCode ec;
        socket.connect(ep, ec);
        if(auto callback = callback_.lock()) {
            if(ec) {
                callback->set_address("Unable to gather LAN address");
            } else {
                callback->set_address(socket.local_endpoint().address().to_string());
            }
        }
    });
}

