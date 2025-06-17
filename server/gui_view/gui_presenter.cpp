#include "gui_presenter.hpp"
#include "filesize_formatter.hpp"

GUIPresenter::GUIPresenter(int& argc, char** argv) :
    application_(argc, argv),
    window_()
    {}
int GUIPresenter::run() {
    window_.set_callback(shared_from_this());
    window_.show();
    return application_.exec();
}

void GUIPresenter::listen() {
    callback_->listen(window_.get_port());
}
void GUIPresenter::set_progressbar(double persent) {
    QMetaObject::invokeMethod(&window_, [this, persent] {
        window_.set_progressbar(persent);
    });
}
void GUIPresenter::set_address(const Address& address) {
    QMetaObject::invokeMethod(&window_, [this, address] {
        window_.set_ipaddress(address.c_str());
    });
}
void GUIPresenter::cant_open_socket() {
    QMetaObject::invokeMethod(&window_, [this] {
        window_.cant_open_socket();
    });
}

void GUIPresenter::connection_opened(const Address& address, Port port) {
    QMetaObject::invokeMethod(&window_, [this, address, port] {
        window_.on_connection_opened(address.c_str(), QString::number(port));
    });
}
void GUIPresenter::connection_aborted(const Address& address, Port port) {
    QMetaObject::invokeMethod(&window_, [this, address, port] {
        window_.on_connection_aborted(address.c_str(), QString::number(port));
    });
}
void GUIPresenter::connected(const Address& address, Port port) {
    QMetaObject::invokeMethod(&window_, [this, address, port] {
        window_.on_connected(address.c_str(), QString::number(port));
    });
}
void GUIPresenter::file_transfered() {
    QMetaObject::invokeMethod(&window_, [this] {
        window_.on_file_transfered();   
    });
}
bool GUIPresenter::verify_file(SendRequest send_request) {
    bool result;
    QMetaObject::invokeMethod(&window_, [this, &result, send_request] {
        result = window_.verify_file(send_request.filename.c_str(), filesize_to_qstring(send_request.filesize));
    }, Qt::BlockingQueuedConnection);
    return result;
}
QString GUIPresenter::filesize_to_qstring(Filesize filesize) {
    return FilesizeFormatter::to_string(filesize).c_str();
}
