#pragma once
#include "port.hpp"

namespace general {
namespace server {
namespace view {

class ViewCallback {
public:
    virtual void listen(Port port) = 0;
    virtual ~ViewCallback() = default;
};

}
}
}
