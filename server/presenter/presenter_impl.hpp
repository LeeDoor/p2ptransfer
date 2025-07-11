#pragma once

#include "presenter.hpp"
#include "view.hpp"
class PresenterImpl : 
    public Presenter, 
    public std::enable_shared_from_this<PresenterImpl> {
public:
    PresenterImpl(std::shared_ptr<Listener> listener,
                  std::shared_ptr<AddressGatherer> address_gatherer,
                  std::shared_ptr<View> view);
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
    bool is_initialized_ = false;
    std::shared_ptr<View> view_;
    std::shared_ptr<Listener> listener_;
    std::shared_ptr<AddressGatherer> address_gatherer_;
};
