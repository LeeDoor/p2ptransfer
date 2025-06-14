#pragma once
#include "headers.hpp"
#include "server/model_network_manager.hpp"
class NetworkManagerMock : public IModelNetworkManager {
public:
    MOCK_METHOD(int, init, (Port port), (override));
};
