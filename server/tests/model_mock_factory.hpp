#pragma once

#include "model_factory.hpp"
class ModelMockFactory : public ModelFactory {
public:
    ModelMockFactory(
        std::shared_ptr<Listener> listener,
        std::shared_ptr<AddressGatherer> address_gatherer
    ) : 
        listener_(listener),
        address_gatherer_(address_gatherer)
    {}
    std::shared_ptr<Listener> create_listener() override;
    std::shared_ptr<AddressGatherer> create_address_gatherer() override;
private:
    std::shared_ptr<Listener> listener_;
    std::shared_ptr<AddressGatherer> address_gatherer_;
};
