#include "server_presenter_impl.hpp"

namespace general {
namespace server {
namespace presenter {

PresenterImpl::PresenterImpl(ListenerPtr listener,
                             AddressGathererPtr address_gatherer,
                             ViewPtr view) :
    Presenter{address_gatherer, view},
    listener_ {listener}
{}

void PresenterImpl::setup()  {
    Presenter::setup();
    listener_->set_callback(shared_from_this());
}
void PresenterImpl::stop()  {
    Presenter::stop();
    listener_->stop();
}

void PresenterImpl::listen(Port port) {
    listener_->listen_if_not_already(port);
}
bool PresenterImpl::verify_file(const Filename& filename, Filesize filesize) {
    return view_->ask_file_verification(filename, filesize);
}

}
}
}
