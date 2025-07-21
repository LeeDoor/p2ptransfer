#pragma once
#include "client_view_callback.hpp"
#include "view.hpp"

namespace general {
namespace client {
namespace view {

class ClientView : public general::view::View<ClientViewCallback> {
public:
    virtual ~ClientView() = default;

    virtual int run() = 0;
    virtual void stop() = 0;
};

}
}
}
