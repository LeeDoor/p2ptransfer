#include "general_view_gui.hpp"
#include "ui_view_gui.h"

namespace general {
namespace server {
namespace view {

GeneralViewGUI::GeneralViewGUI(std::shared_ptr<QApplication> application)
    : QMainWindow{nullptr}
    , ui_{std::make_shared<Ui::GeneralViewGUI>()}
    , application_{application}
    , action_{Listen}
    , selected_file_{""}
{
    ui_->setupUi(this);
    setAcceptDrops(true);
}

void GeneralViewGUI::dragEnterEvent(QDragEnterEvent* event) {
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void GeneralViewGUI::dropEvent(QDropEvent* event) {
    auto urls = event->mimeData()->urls();
    for(QUrl url : urls) {
        set_file_if_accessible(url.toLocalFile());
    }
    event->acceptProposedAction();
}

int GeneralViewGUI::run() {
    QMainWindow::show();
    if(auto application = application_.lock()) {
        return application->exec();
    } else {
        throw std::logic_error("application removed before ViewGUI::run");
    }
}

void GeneralViewGUI::stop() {
    if(auto application = application_.lock()) {
        application->quit();
    } else {
        throw std::logic_error("application removed before ViewGUI::stop");
    }
}

void GeneralViewGUI::show_address(const Address& address) {
    QString qaddress = address.c_str();
    run_sync([=, this] {
        ui_->addressLabel->setText(qaddress);
    });
}

void GeneralViewGUI::update_progressbar_status(double persent) {
    run_sync([=, this] {
        ui_->progressBar->setValue(persent);
    });
}

void GeneralViewGUI::show_connected(const Address& address, Port port) {
    QString qaddress = address.c_str();
    QString qport = QString::number(port);
    run_sync([=, this] {
        ui_->actionButton->setText("Connected to " + qaddress + ":" + qport);
    });
}

void GeneralViewGUI::show_connection_aborted(const Address& address, Port port) {
    QString qaddress = address.c_str();
    QString qport = QString::number(port);
    run_sync([=, this] {
        QMessageBox::warning(this, "Aborted", "connection to " + qaddress + ":" + qport + " is aborted.");
        enable_ui();
    });
}

void GeneralViewGUI::show_file_success() {
    run_sync([=, this] {
        enable_ui();
        QMessageBox::information(this, "Success", "File transferred successfully.");
    });
}
void GeneralViewGUI::show_socket_error() {
    run_sync([=, this] {
        QMessageBox::warning(this, "Socket failure", "Unable to open socket. Please change port and try again.");
        enable_ui();
    });
}
std::shared_ptr<Ui::GeneralViewGUI> GeneralViewGUI::get_ui() {
    return ui_;
}
void GeneralViewGUI::action_button_clicked() {
    disable_ui();
    try {
        if(is_listen()) {
            emit listening(get_port());
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
Port GeneralViewGUI::get_port() const {
    QString text;
    if(is_listen()) {
        text = ui_->listenPortLineEdit->text();
    } else if (is_transfer()) {
        text = ui_->transferPortLineEdit->text();
    }
    bool ok;
    Port port = text.toUInt(&ok);
    if(!ok) throw std::runtime_error("port is not an integer");
    if(port > 65535) {
        throw std::runtime_error("Port must be valid");
    }
    return port;
}
Address GeneralViewGUI::get_address() const {
    if(!is_transfer())
        throw std::logic_error("Can't get address while not transfering tab opened");
    QString ip = ui_->addressLineEdit->text();
    QRegularExpression ip_regex("^((25[0-5]|(2[0-4]|1\\d|[1-9]|)\\d)\\.?\\b){4}$");
    QRegularExpressionValidator validator(ip_regex);
    int pos = 0;
    if(validator.validate(ip, pos) != QValidator::Acceptable) {
        throw std::runtime_error("No ip provided or ip is incorrect");
    }
    return ip.toStdString();
}

void GeneralViewGUI::select_file_button_clicked() {
    QString filepath = QFileDialog::getOpenFileName(this);
    if(filepath.isEmpty())
        return;
    set_file_if_accessible(filepath);
}

void GeneralViewGUI::set_file_if_accessible(QString filepath) {
    if(QFileInfo{filepath}.isDir()) return;
    std::ifstream ofs(filepath.toStdString(), std::ios::binary);
    if(!ofs.is_open()) return;
    selected_file_ = filepath;
    prepare_ui();
}

void GeneralViewGUI::action_changed(int index) {
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

void GeneralViewGUI::disable_ui() {
    ui_->actionButton->setEnabled(false);
    ui_->tabWidget->setEnabled(false);
}

void GeneralViewGUI::enable_ui() {
    ui_->actionButton->setEnabled(true);
    ui_->tabWidget->setEnabled(true);
    ui_->progressBar->setValue(0);
    prepare_ui();
}

void GeneralViewGUI::prepare_ui() {
    switch(action()) {
        case Listen:
            ui_->actionButton->setText("Listen");
            break;
        case Transfer:
            ui_->actionButton->setText("Transfer " + QFileInfo{selected_file_}.fileName());
            break;
    }
}

void GeneralViewGUI::closeEvent([[maybe_unused]] QCloseEvent* e) {
    std::raise(SIGINT);
}
GeneralViewGUI::Action GeneralViewGUI::action() const {
    return action_;
}
bool GeneralViewGUI::is_listen() const {
    return action() == Listen;
}
bool GeneralViewGUI::is_transfer() const {
    return action() == Transfer;
}

}
}
}
