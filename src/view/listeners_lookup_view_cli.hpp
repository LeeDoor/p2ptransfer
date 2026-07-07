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
    void start_lookup() override;
    Endpoint select_remote_endpoint() override;
    void endpoints_responced(Endpoints&& endpoints) override;

private:
    Endpoint select_endpoint();
    void update_table_on_screen() const;
    void refresh();
    size_t make_selection();

    std::shared_ptr<GeneralViewCLI> general_view_;
    Endpoints endpoints_;
    std::unordered_map<Address, Port> stored_endpoints_;
};

}
}


