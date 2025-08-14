#include "client_presenter.hpp"

namespace p2ptransfer {
namespace client {
namespace presenter {

ClientPresenter::ClientPresenter(AddressGathererPtr address_gatherer, ViewPtr view) 
    : Presenter{address_gatherer, view}
{}

void ClientPresenter::send_file_to(Filename filename, Address address, Port port) {
    std::cout << "send_file_to call " + filename + " to " + address + ":" + std::to_string(port) << std::endl;
}

}
}
}
