#pragma once
#include "general_view_gui.hpp"
#include "listener_view.hpp"

namespace general {
namespace server {
namespace view {

class ListenerViewGUI : public QObject, public ListenerView {
    Q_OBJECT
public:
    ListenerViewGUI(std::shared_ptr<GeneralViewGUI> view);

    void setup() override;
    void stop() override;
    bool ask_file_verification(const Filename& filename, Filesize filesize) override;
public slots:
    void listening(Port port);

private:
    std::shared_ptr<GeneralViewGUI> general_view_;
};

}
}
}
