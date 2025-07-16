#pragma once
#include "gather_address_callback.hpp"

namespace general {
namespace test {

class GatherAddressCallbackMock : public GatherAddressCallback {
public:
    MOCK_METHOD(void, set_address, (const Address& address), (noexcept, override));
};

}
}
