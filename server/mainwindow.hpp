#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
public slots:
    void set_ipaddress(const QString& ipaddress);
    void set_progressbar(double persent);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
