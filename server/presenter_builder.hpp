#pragma once
#include "presenter.hpp"

class PresenterBuilder {
public:
    using ProductType = std::shared_ptr<Presenter>;
    ProductType build_GUI(int argc, char** argv);
    // for tests only
    ProductType Build(std::shared_ptr<IView> view,
                      std::shared_ptr<IModelNetworkManager> network,
                      std::shared_ptr<IModelAddressGatherer> address_gatherer);
private:
    ProductType product_;
};
