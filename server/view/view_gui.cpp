#include "view_gui.hpp"
#include "filesize_formatter.hpp"
#include "ui_view_gui.h"

ViewGUI::ViewGUI(std::unique_ptr<QApplication> application, QWidget *parent)
    : QMainWindow(parent)
    , application_(std::move(application))
    , ui(new Ui::ViewGUI)
{
    ui->setupUi(this);
}
ViewGUI::~ViewGUI()
{
    delete ui;
}
int ViewGUI::run() {
    QMainWindow::show();
    return application_->exec();
}
void ViewGUI::stop() {
    application_->quit();
}
void ViewGUI::show_address(const Address& address) {
    QString qaddress = address.c_str();
    run_sync([=, this] {
        ui->ipaddressLabel->setText(qaddress);
    });
}
void ViewGUI::update_progressbar_status(double persent) {
    run_sync([=, this] {
        ui->progressBar->setValue(persent);
    });
}
void ViewGUI::show_connected(const Address& address, Port port) {
    QString qaddress = address.c_str();
    QString qport = QString::number(port);
    run_sync([=, this] {
        ui->buttonListen->setText("Connected to " + qaddress + ":" + qport);
    });
}
void ViewGUI::show_connection_aborted(const Address& address, Port port) {
    QString qaddress = address.c_str();
    QString qport = QString::number(port);
    run_sync([=, this] {
        QMessageBox::warning(this, "Aborted", "connection to " + qaddress + ":" + qport + " is aborted.");
        enable_ui();
    });
}
void ViewGUI::show_file_success() {
    run_sync([=, this] {
        enable_ui();
        QMessageBox::information(this, "Success", "File transferred successfully.");
    });
}
bool ViewGUI::ask_file_verification(const Filename& filename, Filesize filesize) {
    QString qfilename = filename.c_str();
    QString qfilesize = FilesizeFormatter::to_string(filesize).c_str();
    bool confirmed;
    run_sync([=, &confirmed, this] {
        auto result = QMessageBox::question(this, "File request", 
                                            "Do you want to get a file: " + qfilename + " (" + qfilesize + ")?");
        confirmed = result == QMessageBox::Yes;
    });
    return confirmed;
}
void ViewGUI::show_socket_error() {
    run_sync([=, this] {
        QMessageBox::warning(this, "Socket failure", "Unable to open socket. Please change port and try again.");
        enable_ui();
    });
}

Port ViewGUI::get_port() {
    QString text = ui->portLineEdit->text();
    bool ok;
    Port port = text.toUInt(&ok);
    if(!ok) throw std::runtime_error("port is not an integer");
    if(port > 65535) {
        QMessageBox::warning(this, "Wrong Port value", "Port value must be valid.");
    }
    return port;
}

void ViewGUI::listen_pressed() {
    disable_ui();
    callback()->listen(get_port());
}
void ViewGUI::disable_ui() {
    ui->portLineEdit->setEnabled(false);
    ui->buttonListen->setEnabled(false);
}
void ViewGUI::enable_ui() {
    ui->buttonListen->setText("Listen");
    ui->buttonListen->setEnabled(true);
    ui->portLineEdit->setEnabled(true);
    ui->progressBar->setValue(0);
}

void ViewGUI::closeEvent([[maybe_unused]] QCloseEvent* e) {
    std::raise(SIGINT);
}
