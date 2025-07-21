#pragma once
#include "listener.hpp"
#include "presenter.hpp"
#include "server_view.hpp"
#include "server_view_callback.hpp"

namespace general {
namespace server {
namespace presenter {

class PresenterImpl :
        public general::presenter::Presenter<view::ServerViewCallback, server::view::ServerView> {
public:
    using ListenerPtr = std::shared_ptr<model::Listener>;

    PresenterImpl(ListenerPtr listener,
                  AddressGathererPtr address_gatherer,
                  ViewPtr view);

    void setup() override;
    void stop() override;

    bool verify_file(const Filename& filename, Filesize filesize) override;
    void listen(Port port) override;

private:
    ListenerPtr listener_;
};

}
}
}
