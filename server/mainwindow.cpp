#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
void MainWindow::set_ipaddress(const QString& ipaddress) {
    ui->ipaddressLabel->setText(ipaddress);
}

MainWindow::~MainWindow()
{
    delete ui;
}
