#pragma once
#include "gather_address_callback.hpp"

namespace general {
namespace address_gatherer {
namespace test {

class GatherAddressCallbackMock : public GatherAddressCallback {
public:
    MOCK_METHOD(void, set_address, (const Address& address), (override));
};

}
}
}
