#include "general_view_gui.hpp"
#include "speed_converter.hpp"
#include "ui_view_gui.h"

namespace p2ptransfer {
namespace view {

GeneralViewGUI::GeneralViewGUI(std::shared_ptr<QApplication> application)
    : QMainWindow{nullptr}
    , ui_{std::make_shared<Ui::GeneralViewGUI>()}
    , application_{application}
    , action_{Listen}
    , selected_file_{""}
#ifndef NDEBUG
    , main_thread_id_{std::this_thread::get_id()}
#endif
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
        running_ = true;
        lookupping();
        return application->exec();
    } else {
        throw std::logic_error("application removed before ViewGUI::run");
    }
}
void GeneralViewGUI::stop() {
    if(auto app = application_.lock()) {
        running_ = false;
        app->quit();
    }
}

void GeneralViewGUI::show_address(const Address& address) {
    QString qaddress = address.c_str();
    run_sync([=, this] {
        ui_->addressLabel->setText(qaddress);
    });
}

void GeneralViewGUI::update_progressbar_status(double persent, double kbps) {
    run_sync([=, this] {
        ui_->progressBar->setValue(persent);
        QString persent_str = QString::number(persent, 'f', 2);
        QString speed_str = QString::fromStdString(SpeedConverter::from_kbps(kbps));
        ui_->progressBar->setFormat(persent_str + "%\t" + speed_str);
    });
}

void GeneralViewGUI::show_connected(const Address& address, Port port) {
    QString qaddress = address.c_str();
    QString qport = QString::number(port);
    run_sync([=, this] {
        ui_->remote_endpoint_label->setText(qaddress + ":" + qport);
    });
}

void GeneralViewGUI::show_transfer_failed(const Address& address, Port port, std::string reason) {
    QString qaddress = address.c_str();
    QString qport = QString::number(port);
    QString qreason = reason.c_str();
    run_sync([=, this] {
        QMessageBox::warning(this, "Aborted", "connection to " + qaddress + ":" + qport + " is aborted.\n[" + qreason + "]");
        input_waiting_stage();
    });
}

void GeneralViewGUI::show_file_success() {
    run_sync([=, this] {
        QMessageBox::information(this, "Success", "File transferred successfully.");
        input_waiting_stage();
    });
}
void GeneralViewGUI::show_socket_error(std::string reason) {
    run_sync([=, this] {
        QMessageBox::warning(this, "Socket failure", 
                "Unable to open socket. "
                "Please ask your friend to press \"Listen\" first. "
                "If it doesn't help, try to change port.\n"
                "Note that ports should be same on both sides.\n\n["
                + QString{ reason.c_str() } + "]"
        );
        input_waiting_stage();
    });
}
std::shared_ptr<Ui::GeneralViewGUI> GeneralViewGUI::get_ui() {
    return ui_;
}
void GeneralViewGUI::action_button_clicked() {
    if(transferring_) {
        cancel_action();   
    } else {
        perform_action();
    }
}

void GeneralViewGUI::cancel_action() {
    if(is_listen()) {
        cancel_listening();
    } else if (is_transfer()) {
        cancel_transferring();
    }
}

void GeneralViewGUI::perform_action() {
    acting_stage();
    try {
        if(is_listen()) {
            listening(get_listener_port());
        }
        else if (is_transfer()) {
            if(selected_file_.isEmpty()) 
                throw std::runtime_error(
                        "No file selected. Please drag your file to "
                        "this window or press the \"select file\" button.");
            auto endpoint = select_remote_endpoint();
            transferring(endpoint.address, endpoint.port, selected_file_.toStdString());
        }
    } catch (const std::runtime_error& ex) {
        QMessageBox::warning(this, "Action error", ex.what());
        input_waiting_stage();
    }   
}
Endpoint GeneralViewGUI::select_remote_endpoint() const {
    auto table = ui_->tableWidget;
    auto selected_rows = table->selectionModel()->selectedRows();
    if(selected_rows.size() != 1)
        throw std::runtime_error("Please select endpoint to connect to");
    auto row = selected_rows.first().row();
    Endpoint endpoint;
    endpoint.address = table->item(row, 0)->text().toStdString();
    endpoint.port = table->item(row, 1)->text().toUInt();
    return endpoint;
}

Port GeneralViewGUI::get_listener_port() const {
    QString text;
    if(!is_listen()) {
        throw std::runtime_error("getting port from listener's label");
    }
    text = ui_->listenPortLineEdit->text();
    bool ok;
    Port port = text.toUInt(&ok);
    if(!ok) throw std::runtime_error("port is not an integer");
    if(port > 65535) {
        throw std::runtime_error("Port must be valid");
    }
    return port;
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
    rename_action_button();
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
    rename_action_button();
}

void GeneralViewGUI::copy_lan_clicked() {
    auto clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui_->addressLabel->text());
    QString button_text = "Copy"; int timeout_ms = 500;
    if(ui_->addressLabel->text() == clipboard->text()) {
        button_text = "Copied";
        timeout_ms = 500;
    } else {
        button_text = "Can't copy";
        timeout_ms = 2000;
    }
    ui_->copyAddressButton->setText(button_text);
    QTimer::singleShot(timeout_ms, this, [&] {
            ui_->copyAddressButton->setText("Copy");
            });
}

void GeneralViewGUI::acting_stage() {
    transferring_ = true;
    ui_->tabWidget->setEnabled(false);
    rename_action_button();
}

void GeneralViewGUI::input_waiting_stage() {
    transferring_ = false;
    ui_->tabWidget->setEnabled(true);
    ui_->progressBar->setValue(0);
    ui_->progressBar->setFormat("...");
    ui_->remote_endpoint_label->setText("Here will be your firend's IP");
    rename_action_button();
}

void GeneralViewGUI::rename_action_button() {
    if(transferring_) {
        ui_->actionButton->setText("Cancel");
        return;
    }
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
    if(e->spontaneous()) { // Cross X pressed
        std::raise(SIGINT);
    }
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
