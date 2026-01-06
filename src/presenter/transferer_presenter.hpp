#pragma once
#include "request_structures.hpp"
#include "transferer_view_callback.hpp"

namespace p2ptransfer {
namespace view { class TransfererView; }
namespace model { class Transferer; }
namespace presenter {

class NetworkStatusCallback;

/// Special Presenter class for Transferer module.
class TransfererPresenter : 
    public view::TransfererViewCallback, 
    public std::enable_shared_from_this<TransfererPresenter> {
public:
    using NetworkStatusCallbackPtr = std::shared_ptr<NetworkStatusCallback>;
    using TransfererPtr = std::shared_ptr<model::Transferer>;
    using ViewPtr = std::shared_ptr<view::TransfererView>;

    TransfererPresenter(ViewPtr view, TransfererPtr transferer,
                        NetworkStatusCallbackPtr network_callback);

    /// Need to be called before class usage.
    void setup();
    /// Starts the process of transfering file to remote user.
    void transfer(const Address& address, Port port, const Filename& filename) override;
    /// Stops transfering process.
    void cancel_transferring() override;

private:
    ViewPtr view_;
    TransfererPtr transferer_;
    NetworkStatusCallbackPtr network_callback_;

    /// Flag to prevent the class usage unsetupped. 
    bool setupped_ = false;
};

}
}
