#pragma once
#include "view.hpp"

namespace Ui {
class ViewGUI;
}

namespace general {
namespace server {
namespace view {

class ViewGUI : QMainWindow, public View {
    Q_OBJECT
public:
    explicit ViewGUI(std::shared_ptr<QApplication> application, QWidget *parent = nullptr);
    ~ViewGUI();

    int run() override;
    void stop() override;

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
    void closeEvent(QCloseEvent* e) override;

    void setup_ui();
    Port get_port();
    void disable_ui();
    void enable_ui();

    template<typename Func>
    void run_sync(Func&& function) {
        QMetaObject::invokeMethod(this, std::move(function), Qt::BlockingQueuedConnection);
    }

    Ui::ViewGUI *ui;
    std::weak_ptr<QApplication> application_;
};

}
}
}
