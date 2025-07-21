#pragma once
#include "address_gatherer.hpp"
#include "gather_address_callback.hpp"
#include "remote_interaction_callback.hpp"
#include "view.hpp"
#include "view_callback.hpp"

namespace general {
namespace presenter {

template<typename ViewT, typename ViewCallbackT>
concept ViewType = std::derived_from<ViewT, view::View<ViewCallbackT>>;
/*!
* \brief presenter class of MVP pattern.
*
* Performs communicaiton between \ref view::View and Model's \ref model::Listener and \ref AddressGatherer.
* Implements callback interfaces to communicate with each of them.
*/
template<view::ViewCallbackType ViewCallback, ViewType<ViewCallback> View>
class Presenter :
    public GatherAddressCallback,
    public ViewCallback,
    public RemoteInteractionCallback,
    public std::enable_shared_from_this<Presenter<ViewCallback, View>> {

public:
    using AddressGathererPtr = std::shared_ptr<AddressGatherer>;
    using ViewPtr = std::shared_ptr<View>;

    virtual ~Presenter() = default;

    /// Should be called after initialization before run.
    virtual void setup() {
        is_initialized_ = true;
        view_->set_callback(this->shared_from_this());
        address_gatherer_->set_callback(this->shared_from_this());
    }
    /// Starts infinite execution loop, which can be stoppped with \ref stop() method. 
    /// Should be called after \ref setup(). 
    virtual int run() {
        if(!is_initialized_) 
            throw std::runtime_error("Should call Presenter::setup() before run");
        address_gatherer_->gather_local_address();
        return view_->run();
    }
    /// Prepares objects' for stopping. Calling View's and Model's stop() methods.
    virtual void stop() {
        address_gatherer_->stop();
        view_->stop();
    }

    void set_progressbar(double persent) {
        view_->update_progressbar_status(persent);
    }
    void set_address(const Address& address) {
        view_->show_address(address);
    }
    void cant_open_socket() {
        view_->show_socket_error();
    }

    void connection_aborted(const Address& address, Port port) {
        view_->show_connection_aborted(address, port);
    }
    void connected(const Address& address, Port port) {
        view_->show_connected(address, port);
    }
    void file_transfered() {
        view_->show_file_success();
    }

protected:
    Presenter(AddressGathererPtr ag, ViewPtr v) :
        address_gatherer_(ag), view_(v) {}

    AddressGathererPtr address_gatherer_;
    ViewPtr view_;

private:
    /// Safety bool flag to ensure \ref setup() is called.
    bool is_initialized_ = false;
};

}
}
