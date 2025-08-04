#include "listener_presenter.hpp"

namespace general {
namespace server {
namespace presenter {

ListenerPresenter::ListenerPresenter(GeneralPresenterPtr general_presenter, ListenerPtr listener, ViewPtr view) :
    general_presenter_{general_presenter}, listener_{listener}, view_{view}
{}

void ListenerPresenter::setup() {
    view_->set_callback(shared_from_this());
    view_->setup();
    listener_->set_callback(shared_from_this());
    listener_->set_callback(general_presenter_);
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
