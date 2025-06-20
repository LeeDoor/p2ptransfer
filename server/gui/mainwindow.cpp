#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::set_ipaddress(const QString& ipaddress) {
    ui->ipaddressLabel->setText(ipaddress);
}
void MainWindow::set_progressbar(double persent) {
    ui->progressBar->setValue(persent);
}
void MainWindow::cant_open_socket() {
    QMessageBox::warning(this, "Socket failure", "Unable to open socket. Please change port and try again.");
    enable_ui();
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
    disable_ui();
    if(auto callback = callback_.lock()) {
        callback->listen();
    }
}

void MainWindow::on_connection_opened(const QString& address, const QString& port) {
    ui->buttonListen->setText("Waiting for connection...");
}
void MainWindow::on_connected(const QString& address, const QString& port) {
    ui->buttonListen->setText("Connected to " + address + ":" + port);
}
void MainWindow::on_connection_aborted(const QString& address, const QString& port) {
    QMessageBox::warning(this, "Aborted", "connection to " + address + ":" + port + " is aborted.");
    enable_ui();
}
void MainWindow::on_file_transfered() {
    enable_ui();
    QMessageBox::information(this, "Success", "File transferred successfully.");
}
void MainWindow::disable_ui() {
    ui->portLineEdit->setEnabled(false);
    ui->buttonListen->setEnabled(false);
}
void MainWindow::enable_ui() {
    ui->buttonListen->setText("Listen");
    ui->buttonListen->setEnabled(true);
    ui->portLineEdit->setEnabled(true);
    ui->progressBar->setValue(0);
}
bool MainWindow::verify_file(const QString& filename, const QString& filesize) {
    auto result = QMessageBox::question(this, "File request", "Do you want to get a file: " + filename + " (" + filesize + ")?");
    return result == QMessageBox::Yes;
}
