#include "general_presenter.hpp"
#include "address_gatherer.hpp"
#include "general_view.hpp"

namespace p2ptransfer {
namespace presenter {

GeneralPresenter::GeneralPresenter(AddressGathererPtr address_gatherer, ViewPtr view) :
    address_gatherer_{address_gatherer}, 
    view_{view},
    latest_bar_timestamp_{},
    bytes_downloaded_{0}
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
    filesize_ = filesize;
    auto time_shift = hclock::now() - latest_bar_timestamp_;
    if(time_shift >= std::chrono::milliseconds(500)) {
        double bytes = filesize - (bytes_remaining + bytes_downloaded_);
        double bpmcs = bytes / std::chrono::duration_cast<std::chrono::microseconds>(time_shift).count();
        kbps_ = bpmcs * 1000;
        bytes_downloaded_ = filesize - bytes_remaining;
        latest_bar_timestamp_ = hclock::now();
    }
    double percent = static_cast<double>(filesize - bytes_remaining) / filesize * 100;

    view_->update_progressbar_status(percent, kbps_);

}
void GeneralPresenter::set_address(const Address& address) {
    view_->show_address(address);
}
void GeneralPresenter::cant_open_socket(std::string reason) {
    view_->show_socket_error(reason);
}

void GeneralPresenter::transfer_failed(const Address& address, Port port, std::string reason) {
    view_->show_transfer_failed(address, port, reason);
}
void GeneralPresenter::connection_established(const Address& address, Port port) {
    view_->show_connected(address, port);
}
void GeneralPresenter::transfer_succeed() {
    set_progressbar(0, filesize_);
    view_->show_file_success();
}

}
}

