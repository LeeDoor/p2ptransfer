#pragma once
#include "address_gatherer.hpp"

namespace general {
namespace address_gatherer {
namespace test {

class AddressGathererMock : public AddressGatherer {
public:
    MOCK_METHOD(void, gather_local_address, (), (override));
    MOCK_METHOD(void, stop, (), (override));
    std::weak_ptr<GatherAddressCallback> get_callback() {
        return callback_;
    }
};

}
}
}
