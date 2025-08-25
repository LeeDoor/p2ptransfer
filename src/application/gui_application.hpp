#pragma once
#include "application.hpp"
#include "general_view_gui.hpp"
#include "listener_view_gui.hpp"
#include "transferer_view_gui.hpp"

namespace p2ptransfer {

class GUIApplication {
public:
    GUIApplication(int& argc, char** argv) : 
        qt_application_{std::make_shared<QApplication>(argc, argv)},
        gui_application_{std::make_shared<view::GeneralViewGUI>(qt_application_)},
        application_{
            [this] { return gui_application_; },
            [this] { qt_application_->quit(); }
        }
    {}
    int run() { return application_.run(); }
private:
    std::shared_ptr<QApplication> qt_application_;
    std::shared_ptr<view::GeneralViewGUI> gui_application_;
    Application<view::GeneralViewGUI, view::ListenerViewGUI, view::TransfererViewGUI> application_;
};

}
