#pragma once

#include "listener.hpp"
class ListenerMock : public IListener {
public:
    MOCK_METHOD(void, listen_if_not_already, (Port port), (override));
};
