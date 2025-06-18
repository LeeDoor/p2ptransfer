#include "gui_presenter.hpp"
#include "filesize_formatter.hpp"

GUIPresenter::GUIPresenter(int& argc, char** argv,
                           std::shared_ptr<IListener> listener,
                           std::shared_ptr<IAddressGatherer> address_gatherer) :
    application_(argc, argv),
    window_(std::make_shared<MainWindow>()),
    listener_(std::move(listener)),
    address_gatherer_(std::move(address_gatherer))
{}
std::shared_ptr<GUIPresenter> GUIPresenter::shared_from_this() {
    return static_pointer_cast<GUIPresenter>(Presenter::shared_from_this());
}
void GUIPresenter::setup() {
    setup_callbacks();
    setup_ui();
}
void GUIPresenter::setup_callbacks() {
    Presenter::setup_callbacks(shared_from_this());
    address_gatherer_->set_callback(shared_from_this());
    listener_->set_callback(shared_from_this());
    window_->set_callback(shared_from_this());
}
void GUIPresenter::setup_ui() {
    address_gatherer_->gather_local_address();
}
int GUIPresenter::run() {
    window_->show();
    return application_.exec();
}

void GUIPresenter::listen() {
    listener_->listen(window_->get_port());
}
void GUIPresenter::set_progressbar(double persent) {
    QMetaObject::invokeMethod(window_.get(), [this, persent] {
        window_->set_progressbar(persent);
    });
}
void GUIPresenter::set_address(const Address& address) {
    QMetaObject::invokeMethod(window_.get(), [this, address] {
        window_->set_ipaddress(address.c_str());
    });
}
void GUIPresenter::cant_open_socket() {
    QMetaObject::invokeMethod(window_.get(), [this] {
        window_->cant_open_socket();
    });
}

void GUIPresenter::connection_opened(const Address& address, Port port) {
    QMetaObject::invokeMethod(window_.get(), [this, address, port] {
        window_->on_connection_opened(address.c_str(), QString::number(port));
    });
}
void GUIPresenter::connection_aborted(const Address& address, Port port) {
    QMetaObject::invokeMethod(window_.get(), [this, address, port] {
        window_->on_connection_aborted(address.c_str(), QString::number(port));
    });
}
void GUIPresenter::connected(const Address& address, Port port) {
    QMetaObject::invokeMethod(window_.get(), [this, address, port] {
        window_->on_connected(address.c_str(), QString::number(port));
    });
}
void GUIPresenter::file_transfered() {
    QMetaObject::invokeMethod(window_.get(), [this] {
        window_->on_file_transfered();   
    });
}
bool GUIPresenter::verify_file(SendRequest send_request) {
    bool result;
    QMetaObject::invokeMethod(window_.get(), [this, &result, send_request] {
        result = window_->verify_file(send_request.filename.c_str(), filesize_to_qstring(send_request.filesize));
    }, Qt::BlockingQueuedConnection);
    return result;
}
QString GUIPresenter::filesize_to_qstring(Filesize filesize) {
    return FilesizeFormatter::to_string(filesize).c_str();
}
