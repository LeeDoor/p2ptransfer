#include "view_gui_builder.hpp"
#include "view_gui.hpp"

ViewGUIBuilder::ViewGUIBuilder(int& argc, char** argv) 
: argc_(argc), argv_(argv){}
std::shared_ptr<View> ViewGUIBuilder::create_view() {
    auto application = std::make_unique<QApplication>(argc_, argv_);
    auto view = std::make_shared<ViewGUI>(std::move(application));
    return view;
}
