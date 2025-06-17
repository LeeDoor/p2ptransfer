#pragma once
#include "presenter.hpp"

class PresenterBuilder {
public:
    using ProductType = std::shared_ptr<Presenter>;
    virtual ~PresenterBuilder() = default;
    virtual ProductType build() = 0;
protected:
    ProductType build_impl(std::shared_ptr<IView> view,
                      std::shared_ptr<IModelNetworkManager> network,
                      std::shared_ptr<IModelAddressGatherer> address_gatherer) {
        product_ = std::make_shared<Presenter>(view, network, address_gatherer);
        view->set_view_callback(product_);
        network->set_model_callback(product_);
        address_gatherer->set_model_callback(product_);
        return product_;
    }
    ProductType product_;
};
