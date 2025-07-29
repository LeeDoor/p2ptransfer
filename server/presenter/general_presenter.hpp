#pragma once
#include "address_gatherer.hpp"
#include "address_gatherer_callback.hpp"
#include "network_status_callback.hpp"
#include "general_view.hpp"

namespace general {
namespace server {
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
    public Stoppable,
    public std::enable_shared_from_this<GeneralPresenter> {

public:
    using AddressGathererPtr = std::shared_ptr<AddressGatherer>;
    using ViewPtr = std::shared_ptr<view::GeneralView>;

    GeneralPresenter(AddressGathererPtr address_gatherer, ViewPtr view);

    /// Should be called after initialization before run.
    virtual void setup();
    /// Starts infinite execution loop, which can be stoppped with \ref stop() method. 
    /// Should be called after \ref setup(). 
    virtual int run();
    void stop() override;

    void set_progressbar(double persent) override;
    void set_address(const Address& address) override;
    void cant_open_socket() override;

    void connection_aborted(const Address& address, Port port) override;
    void connected(const Address& address, Port port) override;
    void file_transfered() override;

protected:
    AddressGathererPtr address_gatherer_;
    ViewPtr view_;

private:
    /// Safety bool flag to ensure \ref setup() is called.
    bool is_initialized_ = false;
};

}
}
}
