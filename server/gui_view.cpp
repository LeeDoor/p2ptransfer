#include "gui_view.hpp"
GUIView::GUIView(int& argc, char** argv) :
    application_(argc, argv),
    window_()
    {}
int GUIView::run() {
    window_.set_view(shared_from_this());
    window_.show();
    return application_.exec();
}

void GUIView::set_presenter(std::shared_ptr<Presenter> presenter) {
    presenter_ = presenter;
}
void GUIView::listen() {
    if(auto presenter = presenter_.lock()) {
        presenter->listen();
    }
}
void GUIView::set_progressbar(double persent) {
    window_.set_progressbar(persent);
}
void GUIView::set_address(const Address& address) {
    window_.set_ipaddress(address.c_str());
}
Port GUIView::get_port() {
    return window_.get_port();
}

void GUIView::on_connection_opened(const Address& address, Port port) {

}
void GUIView::on_connection_aborted(const Address& address, Port port) {

}
void GUIView::on_connected(const Address& address, Port port) {

}
