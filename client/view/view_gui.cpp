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
    QMessageBox::about(this, "Sending file", selected_file_);
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
