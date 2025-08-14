#pragma once
#include "network_status_callback.hpp"
#include "request_structures.hpp"
#include "stoppable.hpp"
#include "transferer.hpp"
#include "transferer_view.hpp"
#include "transferer_view_callback.hpp"

namespace p2ptransfer {
namespace presenter {

class TransfererPresenter : 
    public view::TransfererViewCallback, 
    public Stoppable,
    public std::enable_shared_from_this<TransfererPresenter> {
public:
    using NetworkStatusCallbackPtr = std::shared_ptr<NetworkStatusCallback>;
    using TransfererPtr = std::shared_ptr<model::Transferer>;
    using ViewPtr = std::shared_ptr<view::TransfererView>;

    TransfererPresenter(ViewPtr view, TransfererPtr transferer,
                        NetworkStatusCallbackPtr network_callback);

    void setup();
    void transfer(const Address& address, Port port, const Filename& filename) override;

private:
    void stop_impl() override;

    ViewPtr view_;
    TransfererPtr transferer_;
    NetworkStatusCallbackPtr network_callback_;
};

}
}
