#include "view_gui.hpp"

int main(int argc, char** argv) {
    auto application = std::make_shared<QApplication>(argc, argv);
    general::client::view::ViewGUI mywindow(application);
    mywindow.set_callback(nullptr);
    return mywindow.run();
}
