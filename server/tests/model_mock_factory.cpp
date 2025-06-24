#include "model_mock_factory.hpp"
#include "address_gatherer_mock.hpp"
#include "listener_mock.hpp"

std::shared_ptr<IListener> ModelMockFactory::create_listener() {
    return std::make_shared<ListenerMock>();
}
std::shared_ptr<IAddressGatherer> ModelMockFactory::create_address_gatherer() {
    return std::make_shared<AddressGathererMock>();
}
