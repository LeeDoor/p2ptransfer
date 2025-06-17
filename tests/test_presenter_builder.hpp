#pragma once
#include "server/presenter_builder.hpp"

class TestPresenterBuilder : public PresenterBuilder {
public:
    TestPresenterBuilder(std::shared_ptr<IView> view,
                         std::shared_ptr<IModelNetworkManager> network,
                         std::shared_ptr<IModelAddressGatherer> address_gatherer)
    : view(view), network(network), address_gatherer(address_gatherer){}
    ProductType build() override {
        return build_impl(view, network, address_gatherer);
    }
protected:
    std::shared_ptr<IView> view;
    std::shared_ptr<IModelNetworkManager> network;
    std::shared_ptr<IModelAddressGatherer> address_gatherer;
};
