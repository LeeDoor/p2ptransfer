#include "view_gui_factory.hpp"
#include "view_gui.hpp"

ViewGUIFactory::ViewGUIFactory(int& argc, char** argv) 
: argc_(argc), argv_(argv){}
std::shared_ptr<View> ViewGUIFactory::create_view() {
    auto application = std::make_unique<QApplication>(argc_, argv_);
    auto view = std::make_shared<ViewGUI>(std::move(application));
    return view;
}
