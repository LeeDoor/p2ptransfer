#include "presenter_builder.hpp"
#include "address_gatherer.hpp"
#include "gui_view.hpp"
#include "network_manager.hpp"
using ProductType = PresenterBuilder::ProductType;
ProductType PresenterBuilder::build_GUI(int argc, char** argv) {
    return Build(std::make_shared<GUIView>(argc, argv),
                 std::make_shared<NetworkManager>(),
                 std::make_shared<AddressGatherer>());
}
ProductType PresenterBuilder::Build(std::shared_ptr<IView> view,
                                    std::shared_ptr<IModelNetworkManager> network,
                                    std::shared_ptr<IModelAddressGatherer> address_gatherer) {
    product_ = std::make_shared<Presenter>(view, network, address_gatherer);
    view->set_view_callback(product_);
    network->set_model_callback(product_);
    address_gatherer->set_model_callback(product_);
    return product_;
}
