#include "view_gui.hpp"
#include "filesize_formatter.hpp"
#include "ui_view_gui.h"

namespace general {
namespace server {
namespace view {

ViewGUI::ViewGUI(std::shared_ptr<QApplication> application)
    : QMainWindow{nullptr}
    , ui{new Ui::ViewGUI}
    , application_{application}
    , action_{Listen}
    , selected_file_{""}
{
    ui->setupUi(this);
    setAcceptDrops(true);
}

ViewGUI::~ViewGUI()
{
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

void ViewGUI::show_address(const Address& address) {
    QString qaddress = address.c_str();
    run_sync([=, this] {
        ui->addressLabel->setText(qaddress);
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
        ui->actionButton->setText("Connected to " + qaddress + ":" + qport);
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

void ViewGUI::action_button_clicked() {
    disable_ui();
    try {
        if(is_listen()) {
            callback()->listen(get_port());
        }
        else if (is_transfer()) {
            if(selected_file_.isEmpty()) 
                throw std::runtime_error(
                    "No file selected. Please drag your file to "
                    "this window or press the \"select file\" button");
        //    callback()->transfer(get_port());
        }
    } catch (const std::runtime_error& ex) {
        QMessageBox::warning(this, "Action error", ex.what());
        enable_ui();
    }   
}

Port ViewGUI::get_port() const {
    QString text;
    if(is_listen()) {
        text = ui->listenPortLineEdit->text();
    } else if (is_transfer()) {
        text = ui->transferPortLineEdit->text();
    }
    bool ok;
    Port port = text.toUInt(&ok);
    if(!ok) throw std::runtime_error("port is not an integer");
    if(port > 65535) {
        throw std::runtime_error("Port must be valid");
    }
    return port;
}
Address ViewGUI::get_address() const {
    if(!is_transfer())
        throw std::logic_error("Can't get address while not transfering tab opened");
    QString ip = ui->addressLineEdit->text();
    QRegularExpression ip_regex("^((25[0-5]|(2[0-4]|1\\d|[1-9]|)\\d)\\.?\\b){4}$");
    QRegularExpressionValidator validator(ip_regex);
    int pos = 0;
    if(validator.validate(ip, pos) != QValidator::Acceptable) {
        throw std::runtime_error("No ip provided or ip is incorrect");
    }
    return ip.toStdString();
}

void ViewGUI::select_file_button_clicked() {
    QString filepath = QFileDialog::getOpenFileName(this);
    if(filepath.isEmpty())
        return;
    set_file_if_accessible(filepath);
}

void ViewGUI::set_file_if_accessible(QString filepath) {
    std::ifstream ofs(filepath.toStdString(), std::ios::binary);
    if(ofs.is_open()) 
        selected_file_ = filepath;
    prepare_ui();
}

void ViewGUI::action_changed(int index) {
    switch(index) {
        case 0:
            action_ = Listen;
            break;
        case 1:
            action_ = Transfer;
            break;
    }
    prepare_ui();
}

void ViewGUI::disable_ui() {
    ui->actionButton->setEnabled(false);
    ui->tabWidget->setEnabled(false);
}

void ViewGUI::enable_ui() {
    ui->actionButton->setEnabled(true);
    ui->tabWidget->setEnabled(true);
    ui->progressBar->setValue(0);
    prepare_ui();
}

void ViewGUI::prepare_ui() {
    switch(action()) {
        case Listen:
            ui->actionButton->setText("Listen");
            break;
        case Transfer:
            ui->actionButton->setText("Transfer " + QFileInfo{selected_file_}.fileName());
            break;
    }
}

void ViewGUI::closeEvent([[maybe_unused]] QCloseEvent* e) {
    std::raise(SIGINT);
}
ViewGUI::Action ViewGUI::action() const {
    return action_;
}
bool ViewGUI::is_listen() const {
    return action() == Listen;
}
bool ViewGUI::is_transfer() const {
    return action() == Transfer;
}

}
}
}
