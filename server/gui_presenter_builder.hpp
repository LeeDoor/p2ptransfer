#pragma once
#include "address_gatherer.hpp"
#include "gui_view.hpp"
#include "network_manager.hpp"
#include "presenter_builder.hpp"

class GUIPresenterBuilder : public PresenterBuilder {
public:
    GUIPresenterBuilder(int argc, char** argv)
        : argc_(argc), argv_(argv){}
    ProductType build() override {
        return build_impl(std::make_shared<GUIView>(argc_, argv_),
                     std::make_shared<NetworkManager>(),
                     std::make_shared<AddressGatherer>());
    }
protected:
    int argc_;
    char** argv_;
};
