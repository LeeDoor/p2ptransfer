#include "transferer_view_gui.hpp"

namespace p2ptransfer {
namespace view {

TransfererViewGUI::TransfererViewGUI(std::shared_ptr<GeneralViewGUI> view) :
    general_view_{view}
{}

void TransfererViewGUI::setup() {
    QObject::connect(general_view_.get(), &GeneralViewGUI::transfering, this, &TransfererViewGUI::transfer);
}
void TransfererViewGUI::transfer(const Address& address, Port port, const Filename& filename) {
    callback()->transfer(address, port, filename);
}

}
}
