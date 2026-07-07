#pragma once
#include "listeners_lookup_view.hpp"

namespace p2ptransfer {
namespace view {

class GeneralViewGUI;

class ListenersLookupViewGUI : public QObject, public ListenersLookupView {
    Q_OBJECT
public:
    ListenersLookupViewGUI(std::shared_ptr<GeneralViewGUI> view_);

    void endpoints_responced(Endpoints&& endpoints) override;
    Endpoint select_remote_endpoint() override { return {}; }
    void setup() override;

public  slots:
    void start_lookup() override;

private:
    std::shared_ptr<GeneralViewGUI> view_;
    std::unordered_map<Address, Port> stored_endpoints_{};
};

}
}

