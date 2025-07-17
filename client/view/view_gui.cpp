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
    event->acceptProposedAction();
}

void ViewGUI::dropEvent(QDropEvent* event) {
    auto list = event->mimeData()->urls();
    for(auto i : list)
        QMessageBox::about(this, "Aboba", i.toString());
    event->acceptProposedAction();
}

}
}
}
