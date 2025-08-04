#pragma once
#include "general_presenter.hpp"
#include "listener.hpp"
#include "listener_view.hpp"
#include "listener_view_callback.hpp"
#include "listener_callback.hpp"

namespace general {
namespace server {
namespace presenter {

class ListenerPresenter final :
    public view::ListenerViewCallback,
    public ListenerCallback,
    public Stoppable,
    public std::enable_shared_from_this<ListenerPresenter> {
public:
    using GeneralPresenterPtr = std::shared_ptr<GeneralPresenter>;
    using ListenerPtr = std::shared_ptr<model::Listener>;
    using ViewPtr = std::shared_ptr<view::ListenerView>;

    ListenerPresenter(GeneralPresenterPtr general, ListenerPtr listener, ViewPtr view);

    void setup();
    void stop() override;
    void listen(Port port) override;
    bool verify_file(const Filename& filename, Filesize filesize) override;

private:
    GeneralPresenterPtr general_presenter_;
    ListenerPtr listener_;
    ViewPtr view_;
};

}
}
}
