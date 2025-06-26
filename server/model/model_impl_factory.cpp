#include "model_impl_factory.hpp"
#include "address_gatherer_impl.hpp"
#include "listener_impl.hpp"

std::shared_ptr<Listener> ModelImplFactory::create_listener() {
    return std::make_shared<ListenerImpl>();
}
std::shared_ptr<AddressGatherer> ModelImplFactory::create_address_gatherer() {
    return std::make_shared<AddressGathererImpl>();
}
