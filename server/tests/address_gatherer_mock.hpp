#pragma once

#include "i_address_gatherer.hpp"
class AddressGathererMock : public IAddressGatherer {
public:
    MOCK_METHOD(void, gather_local_address, (), (override));
};
