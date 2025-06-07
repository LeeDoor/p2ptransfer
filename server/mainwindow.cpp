#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
void MainWindow::set_view(std::shared_ptr<IView> view) {
    view_ = view;
}
void MainWindow::set_ipaddress(const QString& ipaddress) {
    ui->ipaddressLabel->setText(ipaddress);
}
void MainWindow::set_progressbar(double persent) {
    ui->progressBar->setValue(persent);
}
Port MainWindow::get_port() {
    QString text = ui->portLineEdit->text();
    bool ok;
    Port port = text.toUInt(&ok);
    if(!ok) throw std::runtime_error("port is not an integer");
    if(port > 65535) {
        QMessageBox::warning(this, "Wrong Port value", "Port value must be valid.");
    }
    return port;
}

void MainWindow::listen_pressed() {
    if(std::shared_ptr<IView> view = view_.lock()) {
        view->listen();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
