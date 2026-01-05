#pragma once
#include "listener_view_callback.hpp"
#include "listener_callback.hpp"

namespace p2ptransfer {
namespace view { class ListenerView; }
namespace model { class Listener; }
namespace presenter {

class NetworkStatusCallback;
/*! \brief Special Presenter class for \ref Listener module.
 *  
 *  Manages the listnening logic and provides the \ref ListenerCallback
 *  interface for such notifications.
 */
class ListenerPresenter final :
    public view::ListenerViewCallback,
    public ListenerCallback,
    public std::enable_shared_from_this<ListenerPresenter> {
public:
    using NetworkStatusCallbackPtr = std::shared_ptr<NetworkStatusCallback>;
    using ListenerPtr = std::shared_ptr<model::Listener>;
    using ViewPtr = std::shared_ptr<view::ListenerView>;

    ListenerPresenter(NetworkStatusCallbackPtr network_callback, ListenerPtr listener, ViewPtr view);

    /// Should be called before usage. Setups callbacks and the View
    void setup();
    /// Starts listening for connections at given port
    void listen(Port port) override;
    /// Stops listening or file gathering process if initiated already.
    void cancel_listening() override;
    /// Asks user for a file confirmation using \ref ListenerView
    bool verify_file(const Filename& filename, Filesize filesize) override;

private:
    NetworkStatusCallbackPtr network_callback_;
    ListenerPtr listener_;
    ViewPtr view_;

    /// setup verification Flag
    bool setupped_ = false;
};

}
}
