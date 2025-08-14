#pragma once
#include "client_view_callback.hpp"
#include "view.hpp"

namespace p2ptransfer {
namespace client {
namespace view {

class ClientView : public p2ptransfer::view::View<ClientViewCallback> {
public:
    virtual ~ClientView() = default;

    virtual int run() = 0;
    virtual void stop() = 0;
};

}
}
}
