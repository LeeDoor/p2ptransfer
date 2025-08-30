#pragma once
#include "listener_view.hpp"

namespace p2ptransfer {
namespace view {

class GeneralViewGUI;

class ListenerViewGUI : public QObject, public ListenerView {
    Q_OBJECT
public:
    ListenerViewGUI(std::shared_ptr<GeneralViewGUI> view);

    void setup() override;
    bool ask_file_verification(const Filename& filename, Filesize filesize) override;
public slots:
    void listening(Port port);

private:
    std::shared_ptr<GeneralViewGUI> general_view_;
};

}
}
