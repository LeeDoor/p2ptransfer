#include "presenter.hpp"
Presenter::Presenter(std::unique_ptr<IView> view)
    : view_(std::move(view)),
      network_manager_(std::make_unique<NetworkManager>())  {}
int Presenter::run() {
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
void Presenter::Listen() {
    Port port = view_->get_port();
    network_manager_->init(port);
}
