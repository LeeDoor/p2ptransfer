#pragma once
#include "listener_view_callback.hpp"

namespace general {
namespace server {
namespace presenter {

class ListenerPresenter final :
    public view::ListenerViewCallback {

    void listen(Port port) override;
};

}
}
}
