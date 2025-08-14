#pragma once
#include "general_view_gui.hpp"
#include "transferer_view.hpp"

namespace p2ptransfer {
namespace view {

class TransfererViewGUI : public QObject, public TransfererView {
    Q_OBJECT
public:
    TransfererViewGUI(std::shared_ptr<GeneralViewGUI> view);

    void setup() override;
private:
    void transfer(const Address& address, Port port, const Filename& filename);

    std::shared_ptr<GeneralViewGUI> general_view_;
};

}
}
