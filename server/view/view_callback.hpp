#pragma once
#include "port.hpp"

namespace general {
namespace view {

class ViewCallback {
public:
    virtual void listen(Port port) = 0;
    virtual ~ViewCallback() = default;
};

}
}
