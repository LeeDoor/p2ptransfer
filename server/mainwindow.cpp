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

void MainWindow::listen_pressed() {
    if(std::shared_ptr<IView> view = view_.lock()) {
        view->listen();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
