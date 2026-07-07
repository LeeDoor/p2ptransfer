#include "listeners_lookup_view_cli.hpp"
#include "general_view_cli.hpp"

namespace p2ptransfer {
namespace view {

ListenersLookupViewCLI::ListenersLookupViewCLI(std::shared_ptr<GeneralViewCLI> view)
: general_view_{ view }
{}
void ListenersLookupViewCLI::setup() {
    general_view_->subscribe_lookup(std::bind(&ListenersLookupViewCLI::select_remote_endpoint, this));
}
Endpoint ListenersLookupViewCLI::select_remote_endpoint() {
    size_t selected_endpoint = 0;
    update_table_on_screen();
    do {
        refresh();
        selected_endpoint = make_selection();
    } while(selected_endpoint == 0 || selected_endpoint > endpoints_.size());
    auto iter = endpoints_.begin();
    std::advance(iter, selected_endpoint - 1);
    return *iter;
}
void ListenersLookupViewCLI::endpoints_responced(Endpoints&& endpoints) {
    endpoints.remove_if([&] (const auto& ep) {
        bool result =
            stored_endpoints_.contains(ep.address) &&
            stored_endpoints_.at(ep.address) == ep.port;
        if(!result)
            stored_endpoints_[ep.address] = ep.port;
        return result;
    });
    endpoints_.splice(endpoints_.end(), std::move(endpoints));
}

void ListenersLookupViewCLI::update_table_on_screen() const {
    Logger::log() << std::endl;
    Logger::log() << "0) refresh" << std::endl;
    auto endpoint_iter = endpoints_.begin();
    for(size_t i = 1; i <= endpoints_.size(); ++i) {
        const auto& endpoint = *endpoint_iter;
        Logger::log() << i << ") " << endpoint.address << ":" << endpoint.port << std::endl;
        endpoint_iter = std::next(endpoint_iter);
    }
    Logger::log() << "#> ";
}
void ListenersLookupViewCLI::refresh() {
    endpoints_.clear();
    stored_endpoints_.clear();
    callback()->lookup();
    update_table_on_screen();
}

size_t ListenersLookupViewCLI::make_selection() {
    return Logger::enter_positive_integer();
}

}
}
