#include "general_presenter.hpp"
#include "address_gatherer.hpp"
#include "general_view.hpp"

namespace p2ptransfer {
namespace presenter {

GeneralPresenter::GeneralPresenter(AddressGathererPtr address_gatherer, ViewPtr view) :
    address_gatherer_{address_gatherer}, 
    view_{view},
    prev_bar_timestamp_{}, 
    prev_bytes_downloaded_{0},
    prev_kbps_{0}
{}

void GeneralPresenter::setup() {
    is_initialized_ = true;
    address_gatherer_->set_callback(this->shared_from_this());
}
int GeneralPresenter::run() {
    if(!is_initialized_) 
        throw std::logic_error("Should call Presenter::setup() before run");
    address_gatherer_->gather_local_address();
    return view_->run();
}
void GeneralPresenter::stop() {
    address_gatherer_->stop();
    view_->stop();
}
void GeneralPresenter::set_progressbar(size_t bytes_remaining, size_t filesize) {
    double percent = static_cast<double>(filesize - bytes_remaining) / filesize * 100;
    double kbps = prev_kbps_;
    auto time_shift = hclock::now() - prev_bar_timestamp_;
    if(     (prev_bytes_downloaded_ != 0 || // previous value should be set before
             prev_bytes_downloaded_ >= bytes_remaining) && // prevent sutiation if used multiple times
             time_shift > std::chrono::milliseconds(1000)
    ) {
        double bytes = (bytes_remaining - prev_bytes_downloaded_);
        auto bpmcs = bytes / std::chrono::duration_cast<std::chrono::microseconds>(time_shift).count();
        kbps = bpmcs * 1000;
        prev_bar_timestamp_ = hclock::now();
    }
    view_->update_progressbar_status(percent, kbps);
    prev_bytes_downloaded_ = bytes_remaining;
    prev_kbps_ = kbps;
}
void GeneralPresenter::set_address(const Address& address) {
    view_->show_address(address);
}
void GeneralPresenter::cant_open_socket() {
    view_->show_socket_error();
}

void GeneralPresenter::connection_aborted(const Address& address, Port port) {
    view_->show_connection_aborted(address, port);
}
void GeneralPresenter::connected(const Address& address, Port port) {
    view_->show_connected(address, port);
}
void GeneralPresenter::file_transfered() {
    view_->show_file_success();
}

}
}
