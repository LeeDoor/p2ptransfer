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
std::tuple<Address, Port> ListenersLookupViewCLI::select_remote_endpoint() {
    callback()->lookup();
    return { "AAA", 7 };
}

}
}
