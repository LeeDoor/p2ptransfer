#include "model_factory_impl.hpp"
#include "address_gatherer_impl.hpp"
#include "listener_impl.hpp"

std::shared_ptr<IListener> ModelFactoryImpl::create_listener() {
    return std::make_shared<ListenerImpl>();
}
std::shared_ptr<IAddressGatherer> ModelFactoryImpl::create_address_gatherer() {
    return std::make_shared<AddressGathererImpl>();
}
