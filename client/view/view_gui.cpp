#include "view_gui.hpp"
#include "ui_view_gui.h"


namespace p2ptransfer {
namespace client {
namespace view {

ViewGUI::ViewGUI(std::shared_ptr<QApplication> application) :
    QMainWindow{nullptr},
    application_(application),
    ui(new Ui::ViewGUI)
{
    ui->setupUi(this);
    setAcceptDrops(true);
}
ViewGUI::~ViewGUI() {
    delete ui;
}

void ViewGUI::dragEnterEvent(QDragEnterEvent* event) {
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void ViewGUI::dropEvent(QDropEvent* event) {
    auto urls = event->mimeData()->urls();
    for(QUrl url : urls) {
        set_file_if_accessible(url.toLocalFile());
    }
    event->acceptProposedAction();
}

int ViewGUI::run() {
    QMainWindow::show();
    if(auto application = application_.lock()) {
        return application->exec();
    } else {
        throw std::logic_error("application removed before ViewGUI::run");
    }
}
void ViewGUI::stop() {
    if(auto application = application_.lock()) {
        application->quit();
    } else {
        throw std::logic_error("application removed before ViewGUI::stop");
    }
}

void ViewGUI::show_address(Address address) {
    ui->addressLabel->setText(address.c_str());
}
void ViewGUI::connected(Address address, Port port) {
    ui->sendButton->setText("Connected to " + ip_port_to_qstring(address, port));
}
void ViewGUI::connection_failed(Address address, Port port) {
    QMessageBox::warning(this, "Connection failed", 
                         "Connection failed. Check the IP address and port and try again: " + 
                         ip_port_to_qstring(address, port));
    enable_ui();
}
void ViewGUI::update_progressbar(double percent) {
    ui->progressBar->setValue(percent);
}
void ViewGUI::file_transfered() {
    QMessageBox::information(this, "File transfered", "File transferred successfully");
    enable_ui();
}
void ViewGUI::access_denied() {
    QMessageBox::warning(this, "Access denied", "This user denied your files");
    enable_ui();
}
void ViewGUI::connection_aborted() {
    QMessageBox::warning(this, "Connection aborted", "Error occured while file sharing");
    enable_ui();
}

void ViewGUI::set_file_if_accessible(QString filepath) {
    std::ifstream ofs(filepath.toStdString(), std::ios::binary);
    if(ofs.is_open()) 
        selected_file_ = filepath;
    ui->sendButton->setEnabled(true);
    ui->sendButton->setText("Send " + QFileInfo{selected_file_}.fileName());
}

void ViewGUI::send_button_clicked() {
    try {
        disable_ui();
        QString ip = get_validated_ip();
        int port = get_validated_port();
        callback()->send_file_to(selected_file_.toStdString(), ip.toStdString(), port);
    } catch (const std::exception& ex) {
        QMessageBox::warning(this, "Input error", ex.what());
        enable_ui();
    }
}

QString ViewGUI::get_validated_ip() {
    QString ip = ui->addressLineEdit->text();
    QRegularExpression ip_regex("^((25[0-5]|(2[0-4]|1\\d|[1-9]|)\\d)\\.?\\b){4}$");
    QRegularExpressionValidator validator(ip_regex, this);
    int pos = 0;
    if(validator.validate(ip, pos) != QValidator::Acceptable) {
        throw std::runtime_error("No ip provided or ip is incorrect");
    }
    return ip;
}
int ViewGUI::get_validated_port() {
    QString port_str = ui->portLineEdit->text();
    bool ok = false;
    int port = port_str.toInt(&ok);
    if(port < 0 || 65535 < port || !ok)
        throw std::runtime_error("No port provided or port is incorrect");
    return port;
}

void ViewGUI::select_file_button_clicked() {
    QString filepath = QFileDialog::getOpenFileName(this);
    if(filepath.isEmpty())
        return;
    set_file_if_accessible(filepath);
}

void ViewGUI::disable_ui() {
    set_ui_status(false);
}
void ViewGUI::enable_ui() {
    set_ui_status(true);
}
void ViewGUI::set_ui_status(bool enabled) {
    ui->sendButton->setEnabled(enabled);
    ui->selectFileButton->setEnabled(enabled);
    ui->addressLineEdit->setEnabled(enabled);
    ui->portLineEdit->setEnabled(enabled);
}
QString ViewGUI::ip_port_to_qstring(Address address, Port port) {
    return QString{address.c_str()} + ":" + QString::number(port);
}


}
}
}
