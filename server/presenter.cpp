#include "presenter.hpp"
Presenter::Presenter(std::shared_ptr<IView> view, 
              std::shared_ptr<IModelNetworkManager> network_manager,
              std::shared_ptr<IModelAddressGatherer> address_gather)
    : view_(std::move(view)),
      network_manager_(std::move(network_manager)),
      address_gatherer_(std::move(address_gather))  {}
int Presenter::run() {
    address_gatherer_->gather_local_address();
    return view_->run();
}
void Presenter::connection_opened(const Address& address, Port port) {
    view_->connection_opened(address, port);
}
void Presenter::connected(const Address& address, Port port) {
    view_->connected(address, port);
}
void Presenter::connection_aborted(const Address& address, Port port) {
    view_->connection_aborted(address, port);
}
void Presenter::file_transfered() {
    view_->file_transfered();
}
void Presenter::set_progressbar_status(double present) {
    view_->set_progressbar(present);
}
void Presenter::set_address(const Address& address) {
    view_->set_address(address);
}
bool Presenter::verify_file(SendRequest send_request) {
    return view_->verify_file(send_request);
}
void Presenter::cant_open_socket() {
    view_->cant_open_socket();
}
void Presenter::listen() {
    Port port = view_->get_port();
    network_manager_->init(port);
}
