#pragma once

#include "mainwindow.hpp"
#include "qt_headers.hpp"
#include "view.hpp"
class GUIView : public IView, public std::enable_shared_from_this<GUIView> {
public:
    GUIView(int& argc, char** argv);
    int run() override;

    void listen() override;
    void set_progressbar(double persent) override;
    void set_address(const Address& address) override;
    Port get_port() override;
    void cant_open_socket() override;

    void on_connection_opened(const Address& address, Port port) override;
    void on_connection_aborted(const Address& address, Port port) override;
    void on_connected(const Address& address, Port port) override;
    void on_file_transfered() override;
    bool verify_file(SendRequest send_request) override;
private:
    QString filesize_to_qstring(Filesize filesize);
    QApplication application_;
    MainWindow window_;
};
