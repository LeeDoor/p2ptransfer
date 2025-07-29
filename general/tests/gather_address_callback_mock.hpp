#pragma once
#include "address_gatherer_callback.hpp"

namespace general {
namespace test {

class GatherAddressCallbackMock : public GatherAddressCallback {
public:
    MOCK_METHOD(void, set_address, (const Address& address), (override));
};

}
}
