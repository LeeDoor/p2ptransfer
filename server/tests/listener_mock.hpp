#pragma once

#include "listener.hpp"
class ListenerMock : public Listener {
public:
    MOCK_METHOD(void, listen_if_not_already, (Port port), (override));
    MOCK_METHOD(void, stop, (), (override));
    std::weak_ptr<RemoteInteractionCallback> get_callback() {
        return callback_;
    }
};
