#pragma once
#include "listener.hpp"
#include "listener_view.hpp"
#include "listener_view_callback.hpp"
#include "listener_callback.hpp"

namespace p2ptransfer {
namespace presenter {

class ListenerPresenter final :
    public view::ListenerViewCallback,
    public ListenerCallback,
    public Stoppable,
    public std::enable_shared_from_this<ListenerPresenter> {
public:
    using NetworkStatusCallbackPtr = std::shared_ptr<NetworkStatusCallback>;
    using ListenerPtr = std::shared_ptr<model::Listener>;
    using ViewPtr = std::shared_ptr<view::ListenerView>;

    ListenerPresenter(NetworkStatusCallbackPtr network_callback, ListenerPtr listener, ViewPtr view);

    void setup();
    void listen(Port port) override;
    bool verify_file(const Filename& filename, Filesize filesize) override;

private:
    void stop_impl() override;

    NetworkStatusCallbackPtr network_callback_;
    ListenerPtr listener_;
    ViewPtr view_;
};

}
}
