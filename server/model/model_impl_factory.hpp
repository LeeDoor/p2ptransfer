#pragma once

#include "model_factory.hpp"
class ModelImplFactory : public ModelFactory {
public:
    std::shared_ptr<IListener> create_listener() override;
    std::shared_ptr<AddressGatherer> create_address_gatherer() override;
};
