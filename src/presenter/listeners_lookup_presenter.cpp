#include "listeners_lookup_presenter.hpp"
#include "listeners_lookup.hpp"

namespace p2ptransfer {
namespace presenter {

ListenersLookupPresenter::ListenersLookupPresenter(ListenersLookupPtr lookupper)
: listeners_lookup_{ lookupper }
{}

void ListenersLookupPresenter::setup() {
    listeners_lookup_->set_callback(shared_from_this());
    setupped_ = true;
}
void ListenersLookupPresenter::stop() {
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
    Logger::log() << "Failed to make a lookup:" << message << std::endl;
}

}
}
