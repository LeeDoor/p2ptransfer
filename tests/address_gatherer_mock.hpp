#include "server/model_address_gather.hpp"
#include <gmock/gmock.h>

class AddressGathererMock : public IModelAddressGatherer {
public:
    MOCK_METHOD(void, gather_local_address, (), (override));
};
