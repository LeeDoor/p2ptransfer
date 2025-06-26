#include "model_mock_factory.hpp"

std::shared_ptr<Listener> ModelMockFactory::create_listener() {
    return listener_;
}
std::shared_ptr<AddressGatherer> ModelMockFactory::create_address_gatherer() {
    return address_gatherer_;
}
