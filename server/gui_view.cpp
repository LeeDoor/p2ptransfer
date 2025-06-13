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
    QMetaObject::invokeMethod(&window_, [this, persent] {
        window_.set_progressbar(persent);
    });
}
void GUIView::set_address(const Address& address) {
    QMetaObject::invokeMethod(&window_, [this, address] {
        window_.set_ipaddress(address.c_str());
    });
}
Port GUIView::get_port() {
    return window_.get_port();
}
void GUIView::cant_open_socket() {
    QMetaObject::invokeMethod(&window_, [this] {
        window_.cant_open_socket();
    });
}

void GUIView::on_connection_opened(const Address& address, Port port) {
    QMetaObject::invokeMethod(&window_, [this, address, port] {
        window_.on_connection_opened(address.c_str(), QString::number(port));
    });
}
void GUIView::on_connection_aborted(const Address& address, Port port) {
    QMetaObject::invokeMethod(&window_, [this, address, port] {
        window_.on_connection_aborted(address.c_str(), QString::number(port));
    });
}
void GUIView::on_connected(const Address& address, Port port) {
    QMetaObject::invokeMethod(&window_, [this, address, port] {
        window_.on_connected(address.c_str(), QString::number(port));
    });
}
void GUIView::on_file_transfered() {
    QMetaObject::invokeMethod(&window_, [this] {
        window_.on_file_transfered();   
    });
}
bool GUIView::verify_file(SendRequest send_request) {
    bool result;
    QMetaObject::invokeMethod(&window_, [this, &result, send_request] {
        result = window_.verify_file(send_request.filename.c_str(), filesize_to_qstring(send_request.filesize));
    }, Qt::BlockingQueuedConnection);
    return result;
}
QString GUIView::filesize_to_qstring(Filesize filesize) {
    return filesize_to_string(filesize).c_str();
}
