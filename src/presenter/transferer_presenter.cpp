#include "transferer_presenter.hpp"

namespace p2ptransfer {
namespace presenter {

TransfererPresenter::TransfererPresenter(
    ViewPtr view, 
    TransfererPtr transferer,
    NetworkStatusCallbackPtr network_callback)

    : view_(view), 
    transferer_(transferer),
    network_callback_(network_callback) {}

void TransfererPresenter::setup() {
    view_->setup();
    view_->set_callback(shared_from_this());
    transferer_->set_callback(network_callback_);   
}
void TransfererPresenter::transfer(const Address& address, Port port, const Filename& filename) {
    transferer_->transfer_file(address, port, filename);   
}

}
}
