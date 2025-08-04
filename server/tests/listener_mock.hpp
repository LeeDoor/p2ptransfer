#pragma once
#include "listener.hpp"

namespace general {
namespace server {
namespace model {
namespace test {

/// Mock implementation for \ref Listener .
class ListenerMock : public Listener {
public:
    MOCK_METHOD(void, listen_if_not_already, (Port port), (override));
    MOCK_METHOD(void, stop_impl, (), (override));
    std::weak_ptr<presenter::NetworkStatusCallback> get_callback_network() {
        return NetworkStatusCallback::callback_;
    }
    std::weak_ptr<presenter::ListenerCallback> get_callback_listener() {
        return ListenerCallback::callback_;
    }
};

}
}
}
}
