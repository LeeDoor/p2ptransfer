#pragma once
#include "address.hpp"
#include "request_structures.hpp"
#include "client_view.hpp"
#include "presenter.hpp"

namespace p2ptransfer {
namespace client {
namespace presenter {

class ClientPresenter : public p2ptransfer::presenter::Presenter<view::ClientViewCallback, view::ClientView>,
                  public std::enable_shared_from_this<ClientPresenter> {
public:
    using ViewPtr = std::shared_ptr<view::ClientView>;

    ClientPresenter(AddressGathererPtr address_gatherer, ViewPtr view);

    virtual void send_file_to(Filename filename, Address address, Port port) = 0;
};

}
}
}
