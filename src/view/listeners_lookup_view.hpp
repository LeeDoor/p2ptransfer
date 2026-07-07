#pragma once
#include "callback.hpp"
#include "endpoint.hpp"
#include "lookup_view_callback.hpp"

namespace p2ptransfer {
namespace view {

class ListenersLookupView : public WithCallback<LookupViewCallback> {
public:
    virtual ~ListenersLookupView() = default;

    // std::list used for faster container concatenation.
    using Endpoints = std::list<Endpoint>;

    virtual void start_lookup() = 0;
    virtual Endpoint select_remote_endpoint() = 0;
    virtual void endpoints_responced(Endpoints&& endpoints) = 0;
    virtual void setup() = 0;
};

}
}

