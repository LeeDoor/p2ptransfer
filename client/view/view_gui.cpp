#include "view_gui.hpp"
#include "ui_view_gui.h"


namespace general {
namespace client {
namespace view {

ViewGUI::ViewGUI() :
    QMainWindow{nullptr},
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
        set_if_accessible(url.toLocalFile());
    }
    event->acceptProposedAction();
}

void ViewGUI::set_if_accessible(QString filepath) {
    std::ifstream ofs(filepath.toStdString(), std::ios::binary);
    if(ofs.is_open()) 
        selected_file_ = filepath;
    ui->buttonSend->setEnabled(true);
    ui->buttonSend->setText("Send " + QFileInfo{selected_file_}.fileName());
}

void ViewGUI::send_button_clicked() {
    try {
        QString ip = get_validated_ip();
        int port = get_validated_port();
        QMessageBox::about(this, "Sending file", 
                           "Sending " + selected_file_ + 
                           "\nTo " + ip +
                           ":" + QString::number(port));
    } catch (const std::exception& ex) {
        QMessageBox::warning(this, "Input error", ex.what());
    }
}

QString ViewGUI::get_validated_ip() {
    QString ip = ui->IPlineEdit->text();
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
    set_if_accessible(filepath);
}

}
}
}
