#include "gui_view.hpp"
#include "filesize_formatter.hpp"

GUIView::GUIView(int& argc, char** argv) :
    application_(argc, argv),
    window_()
    {}
int GUIView::run() {
    window_.set_callback(shared_from_this());
    window_.show();
    return application_.exec();
}

void GUIView::listen() {
    callback_->listen(window_.get_port());
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
void GUIView::cant_open_socket() {
    QMetaObject::invokeMethod(&window_, [this] {
        window_.cant_open_socket();
    });
}

void GUIView::connection_opened(const Address& address, Port port) {
    QMetaObject::invokeMethod(&window_, [this, address, port] {
        window_.on_connection_opened(address.c_str(), QString::number(port));
    });
}
void GUIView::connection_aborted(const Address& address, Port port) {
    QMetaObject::invokeMethod(&window_, [this, address, port] {
        window_.on_connection_aborted(address.c_str(), QString::number(port));
    });
}
void GUIView::connected(const Address& address, Port port) {
    QMetaObject::invokeMethod(&window_, [this, address, port] {
        window_.on_connected(address.c_str(), QString::number(port));
    });
}
void GUIView::file_transfered() {
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
    return FilesizeFormatter::to_string(filesize).c_str();
}
