#pragma once
#include "callback.hpp"
#include "address.hpp"
#include "port.hpp"
#include "lookup_view_callback.hpp"

namespace p2ptransfer {
namespace view {

class ListenersLookupView : public WithCallback<LookupViewCallback> {
public:
    virtual ~ListenersLookupView() = default;

    virtual void setup() = 0;
    virtual std::tuple<Address, Port> select_remote_endpoint() = 0;
};

}
}

