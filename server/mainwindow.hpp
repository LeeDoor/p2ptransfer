#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "view.hpp"
#include "qt_headers.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void set_view(std::shared_ptr<IView> view);

    void set_ipaddress(const QString& ipaddress);
    void set_progressbar(double persent);
    void on_connection_opened(const QString& address, const QString& port);
    void on_connected(const QString& address, const QString& port);
    void on_connection_aborted(const QString& address, const QString& port);
    void on_file_transfered();
    bool verify_file(const QString& filename, const QString& filesize);

    Port get_port();
public slots:
    void listen_pressed();

private:
    void disable_ui();
    void enable_ui();

    Ui::MainWindow *ui;
    std::weak_ptr<IView> view_;
};

#endif // MAINWINDOW_H
