#pragma once

#include "address_gatherer_builder.hpp"
#include "address_gatherer_mock.hpp"
class AddressGathererMockBuilder : public AddressGathererBuilder {
public:
    std::shared_ptr<AddressGatherer> create_address_gatherer() override {
        return std::make_shared<AddressGathererMock>();
    }
};
