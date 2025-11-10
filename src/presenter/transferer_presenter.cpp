#include "transferer_presenter.hpp"
#include "transferer.hpp"
#include "transferer_view.hpp"
#include "network_status_callback.hpp"

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
    setupped_ = true;
}
void TransfererPresenter::transfer(const Address& address, Port port, const Filename& filename) {
    if(!setupped_) throw std::logic_error("TransfererPresenter: call setup() before usage");
    transferer_->transfer_file(address, port, filename);   
}
void TransfererPresenter::stop() {
    transferer_->stop();
}

}
}
