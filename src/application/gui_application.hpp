#pragma once
#include "application.hpp"
#include "general_view_gui.hpp"
#include "listener_view_gui.hpp"

namespace p2ptransfer {

class GUIApplication {
public:
    GUIApplication(int argc, char** argv) : 
        qapplication_{std::make_shared<QApplication>(argc, argv)},
        application_{[this] { return qapplication_; } }
    {}
    int run() { return application_.run(); }
private:
    std::shared_ptr<QApplication> qapplication_;
    Application<view::GeneralViewGUI, view::ListenerViewGUI> application_;
};

}
