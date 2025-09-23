#include "listener_presenter.hpp"
#include "listener.hpp"
#include "listener_view.hpp"

namespace p2ptransfer {
namespace presenter {

ListenerPresenter::ListenerPresenter(NetworkStatusCallbackPtr network_callback, ListenerPtr listener, ViewPtr view) :
    network_callback_{network_callback}, listener_{listener}, view_{view}
{}

void ListenerPresenter::setup() {
    view_->set_callback(shared_from_this());
    view_->setup();
    listener_->set_callback(shared_from_this());
    listener_->set_callback(network_callback_);
    setupped_ = true;
}
void ListenerPresenter::listen(Port port) {
    if(!setupped_) throw std::logic_error("ListenerPresenter exception: call setup() before usage");
    listener_->listen_if_not_already(port);
}
bool ListenerPresenter::verify_file(const Filename& filename, Filesize filesize) {
    return view_->ask_file_verification(filename, filesize);
}

}
}
