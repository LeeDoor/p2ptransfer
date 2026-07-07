#pragma once
#include "address_gatherer_callback.hpp"
#include "network_status_callback.hpp"

namespace p2ptransfer {

class AddressGatherer;

namespace view { class GeneralView; }

namespace presenter {

/*!
* \brief presenter class of MVP pattern.
*
* Performs communicaiton between \ref view::View and Model's \ref model::Listener and \ref AddressGatherer.
* Implements callback interfaces to communicate with each of them.
*/
class GeneralPresenter final :
    public AddressGathererCallback,
    public NetworkStatusCallback,
    public std::enable_shared_from_this<GeneralPresenter> {

public:
    using AddressGathererPtr = std::shared_ptr<AddressGatherer>;
    using ViewPtr = std::shared_ptr<view::GeneralView>;

    GeneralPresenter(AddressGathererPtr address_gatherer, ViewPtr view);

    /// Should be called after initialization before run.
    void setup();
    /// Starts infinite execution loop, which can be stoppped with \ref stop() method. 
    /// Should be called after \ref setup(). 
    int run();
    /// stops address gatherer execution and general view loop
    void stop();

    void set_progressbar(size_t bytes_remaining, size_t filesize) override;
    void set_address(const Address& address) override;
    void cant_open_socket(std::string reason) override;

    void transfer_failed(const Address& address, Port port, std::string reason) override;
    void connection_established(const Address& address, Port port) override;
    void transfer_succeed() override;

private:
    AddressGathererPtr address_gatherer_;
    ViewPtr view_;

    /// Safety bool flag to ensure \ref setup() is called.
    bool is_initialized_ = false;

    using hclock = std::chrono::high_resolution_clock;
    decltype(hclock::now()) latest_bar_timestamp_;
    size_t bytes_downloaded_ = 0;
    double kbps_ = 0;
    size_t filesize_ = 0;
};

}
}
