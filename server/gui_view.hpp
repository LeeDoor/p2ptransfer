#pragma once

#include "mainwindow.hpp"
#include "qt_headers.hpp"
#include "view.hpp"
class GUIView : public IView {
public:
    GUIView(int& argc, char** argv);
    int run() override;

    void set_progressbar(double persent) override;
    void set_address(const Address& address) override;
    Port get_port() override;

    void on_connection_opened(const Address& address, Port port) override;
    void on_connection_aborted(const Address& address, Port port) override;
    void on_connected(const Address& address, Port port) override;
private:
    QApplication application_;
    MainWindow window_;
};
