#include "listener_presenter.hpp"

namespace general {
namespace server {
namespace presenter {

ListenerPresenter::ListenerPresenter(ListenerPtr listener, ViewPtr view) :
    listener_{listener}, view_{view}
{}

void ListenerPresenter::setup() {
    listener_->set_callback(shared_from_this());
    view_->set_callback(shared_from_this());
    // TODO: listener --->>> general presenter
}
void ListenerPresenter::stop() {
    listener_->stop();
    view_->stop();
}
void ListenerPresenter::listen(Port port) {
    listener_->listen_if_not_already(port);
}
bool ListenerPresenter::verify_file(const Filename& filename, Filesize filesize) {
    return view_->ask_file_verification(filename, filesize);
}

}
}
}
