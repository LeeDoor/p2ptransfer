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
}
ViewGUI::~ViewGUI() {
    delete ui;
}

}
}
}
