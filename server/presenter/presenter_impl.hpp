#pragma once
#include "address_gatherer.hpp"
#include "listener.hpp"
#include "presenter.hpp"
#include "view.hpp"

namespace general {
namespace server {
namespace presenter {

class PresenterImpl : 
    public Presenter, 
    public std::enable_shared_from_this<PresenterImpl> {

public:
    using ListenerPtr = std::shared_ptr<model::Listener>;
    using AddressGathererPtr = std::shared_ptr<AddressGatherer>;
    using ViewPtr = std::shared_ptr<view::View>;

    PresenterImpl(ListenerPtr listener,
                  AddressGathererPtr address_gatherer,
                  ViewPtr view);

    void setup() override;
    int run() override;
    void stop() override;

    void listen(Port port) override;
    void set_progressbar(double persent) override;
    void set_address(const Address& address) override;
    void cant_open_socket() override;

    void connection_aborted(const Address& address, Port port) override;
    void connected(const Address& address, Port port) override;
    void file_transfered() override;
    bool verify_file(const Filename& filename, Filesize filesize) override;

private:
    ListenerPtr listener_;
    AddressGathererPtr address_gatherer_;
    ViewPtr view_;
    /// Safety bool flag to ensure \ref setup() is called.
    bool is_initialized_ = false;
};

}
}
}
