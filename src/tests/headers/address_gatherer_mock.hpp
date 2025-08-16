#pragma once
#include "address_gatherer.hpp"

namespace p2ptransfer {
namespace test {

class AddressGathererMock : public AddressGatherer {
public:
    MOCK_METHOD(void, gather_local_address, (), (override));
    MOCK_METHOD(void, stop_impl, (), (override));
    std::weak_ptr<AddressGathererCallback> get_callback() {
        return callback_;
    }
};

}
}
