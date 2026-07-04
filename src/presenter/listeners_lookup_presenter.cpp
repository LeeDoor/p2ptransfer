#include "listeners_lookup_presenter.hpp"
#include "listeners_lookup.hpp"
#include "listeners_lookup_view.hpp"

namespace p2ptransfer {
namespace presenter {

ListenersLookupPresenter::ListenersLookupPresenter(ViewPtr view, ListenersLookupPtr lookupper)
: view_{ view }
, listeners_lookup_{ lookupper }
{}

void ListenersLookupPresenter::setup() {
    view_->setup();
    listeners_lookup_->set_callback(shared_from_this());
    setupped_ = true;
}
void ListenersLookupPresenter::cancel_lookup() {
    listeners_lookup_->stop();
}

void ListenersLookupPresenter::lookup() {
    if(!setupped_) 
        throw std::logic_error("ListenersLookupPresenter exception: call setup() before usage");
    listeners_lookup_->start_lookup();
}
void ListenersLookupPresenter::responce_received(Address address, Port port) {
    Logger::log() << "Looking up found a listener: " << address << ":" << port << std::endl;
}

void ListenersLookupPresenter::failed_to_lookup(std::string message) {
    Logger::log() << "Failed to make a lookup: " << message << std::endl;
}

}
}
