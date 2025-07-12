#pragma once
#include "callback.hpp"
#include "port.hpp"
#include "remote_interaction_callback.hpp"

namespace general {
namespace server {
namespace model {

class Listener : public WithCallback<RemoteInteractionCallback>{
public:
    virtual ~Listener() = default;
    virtual void listen_if_not_already(Port port) = 0;
    virtual void stop() = 0;
};

}
}
}
