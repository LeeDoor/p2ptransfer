#pragma once

#include "address_gatherer.hpp"
class AddressGathererMock : public AddressGatherer {
public:
    MOCK_METHOD(void, gather_local_address, (), (override));
};
