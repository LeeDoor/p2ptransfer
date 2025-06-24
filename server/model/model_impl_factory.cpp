#include "model_impl_factory.hpp"
#include "address_gatherer_impl.hpp"
#include "listener_impl.hpp"

std::shared_ptr<IListener> ModelImplFactory::create_listener() {
    return std::make_shared<ListenerImpl>();
}
std::shared_ptr<IAddressGatherer> ModelImplFactory::create_address_gatherer() {
    return std::make_shared<AddressGathererImpl>();
}
