#pragma once
#include "transferer_view.hpp"

namespace p2ptransfer {
namespace view {

class GeneralViewGUI;

class TransfererViewGUI : public QObject, public TransfererView {
    Q_OBJECT
public:
    TransfererViewGUI(std::shared_ptr<GeneralViewGUI> view);

    void setup() override;
private:
    void transfer(const Address& address, Port port, const Filename& filename);
    void cancel_transferring();

    std::shared_ptr<GeneralViewGUI> general_view_;
};

}
}
