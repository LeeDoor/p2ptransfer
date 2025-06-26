#include "presenter_impl.hpp"
PresenterImpl::PresenterImpl(std::shared_ptr<ModelFactory> model_factory,
              std::shared_ptr<ViewFactory> view_factory) {
    view_ = view_factory->create_view();
    listener_ = model_factory->create_listener();
    address_gatherer_ = model_factory->create_address_gatherer();
}

void PresenterImpl::setup() {
    is_initialized_ = true;
    view_->set_callback(shared_from_this());
    listener_->set_callback(shared_from_this());
    address_gatherer_->set_callback(shared_from_this());
}
int PresenterImpl::run() {
    if(!is_initialized_) 
        throw std::runtime_error("Should call Presenter::setup() before run");
    address_gatherer_->gather_local_address();
    return view_->run();
}

void PresenterImpl::listen(Port port) {
    listener_->listen_if_not_already(port);
}
void PresenterImpl::set_progressbar(double persent) {
    view_->update_progressbar_status(persent);
}
void PresenterImpl::set_address(const Address& address) {
    view_->show_address(address);
}
void PresenterImpl::cant_open_socket() {
    view_->show_socket_error();
}

void PresenterImpl::connection_aborted(const Address& address, Port port) {
    view_->show_connection_aborted(address, port);
}
void PresenterImpl::connected(const Address& address, Port port) {
    view_->show_connected(address, port);
}
void PresenterImpl::file_transfered() {
    view_->show_file_success();
}
bool PresenterImpl::verify_file(const Filename& filename, Filesize filesize) {
    return view_->ask_file_verification(filename, filesize);
}
