#include "general_presenter.hpp"

namespace general {
namespace server {
namespace presenter {

GeneralPresenter::GeneralPresenter(AddressGathererPtr address_gatherer, ViewPtr view) :
    address_gatherer_{address_gatherer}, view_{view} {}

/// Should be called after initialization before run.
void GeneralPresenter::setup() {
    is_initialized_ = true;
    address_gatherer_->set_callback(this->shared_from_this());
}
/// Starts infinite execution loop, which can be stoppped with \ref stop() method. 
/// Should be called after \ref setup(). 
int GeneralPresenter::run() {
    if(!is_initialized_) 
        throw std::logic_error("Should call Presenter::setup() before run");
    address_gatherer_->gather_local_address();
    return view_->run();
}
/// Prepares objects' for stopping. Calling View's and Model's stop() methods.
void GeneralPresenter::stop() {
    address_gatherer_->stop();
    view_->stop();
}

void GeneralPresenter::set_progressbar(double persent) {
    view_->update_progressbar_status(persent);
}
void GeneralPresenter::set_address(const Address& address) {
    view_->show_address(address);
}
void GeneralPresenter::cant_open_socket() {
    view_->show_socket_error();
}

void GeneralPresenter::connection_aborted(const Address& address, Port port) {
    view_->show_connection_aborted(address, port);
}
void GeneralPresenter::connected(const Address& address, Port port) {
    view_->show_connected(address, port);
}
void GeneralPresenter::file_transfered() {
    view_->show_file_success();
}

}
}
}
