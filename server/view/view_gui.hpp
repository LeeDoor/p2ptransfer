#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "view.hpp"
#include "qt_headers.hpp"

namespace Ui {
class ViewGUI;
}

class ViewGUI : public QMainWindow, public View {
    Q_OBJECT

public:
    explicit ViewGUI(std::unique_ptr<QApplication> application, QWidget *parent = nullptr);
    ~ViewGUI();

    int run() override;

    void show_address(const Address& ipaddress) override;
    void update_progressbar_status(double persent) override;
    void show_connected(const Address& address, Port port) override;
    void show_connection_aborted(const Address& address, Port port) override;
    void show_file_success() override;
    bool ask_file_verification(const Filename& filename, Filesize filesize) override;
    void show_socket_error() override;
public slots:
    void listen_pressed();

private:
    void setup_ui();
    Port get_port();
    void disable_ui();
    void enable_ui();

    template<typename Func>
    void run_sync(Func&& function) {
        QMetaObject::invokeMethod(this, std::move(function), Qt::BlockingQueuedConnection);
    }

    std::unique_ptr<QApplication> application_;
    Ui::ViewGUI *ui;
};

#endif // MAINWINDOW_H
