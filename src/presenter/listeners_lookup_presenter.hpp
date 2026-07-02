#pragma once
#include "lookup_results_callback.hpp"

namespace p2ptransfer {
namespace model { class ListenersLookup; }
namespace presenter {

class NetworkStatusCallback;

class ListenersLookupPresenter final :
    public LookupResultsCallback,
    public std::enable_shared_from_this<ListenersLookupPresenter> {
public:
    using ListenersLookupPtr = std::shared_ptr<model::ListenersLookup>;
    ListenersLookupPresenter(ListenersLookupPtr lookupper);

    void setup();
    void stop();

    void lookup();
    void responce_received(Address address, Port port) override;

private:
    ListenersLookupPtr listeners_lookup_;

    /// setup verification Flag
    bool setupped_ = false;
};

}
}
