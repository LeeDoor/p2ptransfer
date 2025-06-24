#pragma once

#include "model_factory.hpp"
class ModelMockFactory : public ModelFactory {
public:
    std::shared_ptr<IListener> create_listener() override;
    std::shared_ptr<IAddressGatherer> create_address_gatherer() override;
};
