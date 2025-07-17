#include "view_gui.hpp"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    general::client::view::ViewGUI mywindow;
    mywindow.show();
    app.exec();
}
