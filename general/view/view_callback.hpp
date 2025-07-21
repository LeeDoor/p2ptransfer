#pragma once
#include "port.hpp"

namespace general {
namespace view {

/// \brief Interface for getting information about User Input.
class ViewCallback {
public:
    virtual ~ViewCallback() = default;
};

}
}
