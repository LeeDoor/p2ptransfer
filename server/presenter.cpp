#include "presenter.hpp"
Presenter::Presenter(std::shared_ptr<IView> view)
    : view_(std::move(view)),
      network_manager_(std::make_shared<NetworkManager>())  {}
int Presenter::run() {
    view_->set_presenter(shared_from_this());
    network_manager_->set_presenter(shared_from_this());
    return view_->run();
}
void Presenter::connection_opened(const Address& address, Port port) {
    view_->on_connection_opened(address, port);
}
void Presenter::connected(const Address& address, Port port) {
    view_->on_connected(address, port);
}
void Presenter::connection_aborted(const Address& address, Port port) {
    view_->on_connection_aborted(address, port);
}
void Presenter::file_transfered() {
    view_->on_file_transfered();
}
void Presenter::set_progressbar_status(double present) {
    view_->set_progressbar(present);
}
void Presenter::listen() {
    Port port = view_->get_port();
    network_manager_->init(port);
}
