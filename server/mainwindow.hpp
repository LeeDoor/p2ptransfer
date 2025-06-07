#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "view.hpp"
#include <QMainWindow>

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
public slots:
    void listen_pressed();

private:
    Ui::MainWindow *ui;
    std::weak_ptr<IView> view_;
};

#endif // MAINWINDOW_H
