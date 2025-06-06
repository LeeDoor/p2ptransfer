#include "mainwindow.hpp"
#include "qt_headers.hpp"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
