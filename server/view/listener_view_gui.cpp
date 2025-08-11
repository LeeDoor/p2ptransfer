#include "listener_view_gui.hpp"
#include "filesize_formatter.hpp"

namespace general {
namespace server {
namespace view {
ListenerViewGUI::ListenerViewGUI(std::shared_ptr<GeneralViewGUI> view) :
    QObject{nullptr},
    general_view_{view}
{}

void ListenerViewGUI::setup() {
    QObject::connect(general_view_.get(), &GeneralViewGUI::listening, this, &ListenerViewGUI::listening);
}
bool ListenerViewGUI::ask_file_verification(const Filename& filename, Filesize filesize) {
    QString qfilename = filename.c_str();
    QString qfilesize = FilesizeFormatter::to_string(filesize).c_str();
    bool confirmed;
    general_view_->run_sync([=, &confirmed, this] {
        auto result = QMessageBox::question(general_view_.get(), "File request", 
                                            "Do you want to get a file: " + qfilename + " (" + qfilesize + ")?");
        confirmed = result == QMessageBox::Yes;
    });
    return confirmed;
}
void ListenerViewGUI::listening(Port port) {
    callback()->listen(port);
}   

}
}
}
