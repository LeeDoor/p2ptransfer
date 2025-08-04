#pragma once
#include "general_presenter.hpp"
#include "listener_view_callback.hpp"

namespace general {
namespace server {
namespace presenter {

class ListenerPresenter final :
    public view::ListenerViewCallback {
public:
    ListenerPresenter(std::shared_ptr<GeneralPresenter> general_presenter);

    void listen(Port port) override;

private:
    std::shared_ptr<GeneralPresenter> general_presenter_;
};

}
}
}
