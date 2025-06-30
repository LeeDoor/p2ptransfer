#pragma once

#include "gather_address_callback.hpp"
class GatherAddressCallbackMock : public GatherAddressCallback {
public:
    MOCK_METHOD(void, set_address, (const Address& address), (override));
};
