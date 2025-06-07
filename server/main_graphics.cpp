#include "mainwindow.hpp"
#include "qt_headers.hpp"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainWindow window;
    window.set_ipaddress("192.168.1.1");
    window.show();
    return app.exec();
}
