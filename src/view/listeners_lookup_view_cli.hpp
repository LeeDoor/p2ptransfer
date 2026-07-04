#pragma once
#include "listeners_lookup_view.hpp"

namespace p2ptransfer {
namespace view {

class GeneralViewCLI;

class ListenersLookupViewCLI : public ListenersLookupView {
public:
    ListenersLookupViewCLI(std::shared_ptr<GeneralViewCLI> view);
    virtual ~ListenersLookupViewCLI() = default;

    void setup() override;
    std::tuple<Address, Port> select_remote_endpoint() override;

private:
    std::shared_ptr<GeneralViewCLI> general_view_;
};

}
}


