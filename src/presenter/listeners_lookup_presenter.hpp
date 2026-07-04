#pragma once
#include "lookup_results_callback.hpp"
#include "lookup_view_callback.hpp"

namespace p2ptransfer {
namespace view { class ListenersLookupView; }
namespace model { class ListenersLookup; }
namespace presenter {

class NetworkStatusCallback;

class ListenersLookupPresenter final :
    public LookupResultsCallback,
    public view::LookupViewCallback,
    public std::enable_shared_from_this<ListenersLookupPresenter> {
public:
    using ViewPtr = std::shared_ptr<view::ListenersLookupView>;
    using ListenersLookupPtr = std::shared_ptr<model::ListenersLookup>;

    ListenersLookupPresenter(ViewPtr view, ListenersLookupPtr lookupper);

    void setup();
    void cancel_lookup() override;

    void lookup() override;
    void responce_received(Address address, Port port) override;
    void failed_to_lookup(std::string message) override;

private:
    ViewPtr view_;
    ListenersLookupPtr listeners_lookup_;

    /// setup verification Flag
    bool setupped_ = false;
};

}
}
