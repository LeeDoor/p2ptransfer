#include "qt_headers.hpp"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    auto mywindow = new QWidget();
    mywindow->show();
    app.exec();
}
