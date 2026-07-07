#include "listeners_lookup_view_gui.hpp"
#include "general_view_gui.hpp"
#include "ui_view_gui.h"

namespace p2ptransfer {
namespace view {

ListenersLookupViewGUI::ListenersLookupViewGUI(std::shared_ptr<GeneralViewGUI> view)
: view_{ view }
{}

void ListenersLookupViewGUI::setup() {
    QObject::connect(view_.get(), &GeneralViewGUI::lookupping, this, &ListenersLookupViewGUI::start_lookup);
}
void ListenersLookupViewGUI::start_lookup() {
    callback()->lookup();
}
void ListenersLookupViewGUI::endpoints_responced(Endpoints&& endpoints) {
    view_->run_sync([this, eps=std::move(endpoints)] {
        auto& table = view_->get_ui()->tableWidget;
        for(const auto& ep : eps) {
            if (!stored_endpoints_.contains(ep.address) ||
                stored_endpoints_.at(ep.address) != ep.port) {
                auto row = table->rowCount();
                table->insertRow(row);
                table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(ep.address)));
                table->setItem(row, 1, new QTableWidgetItem(QString::number(ep.port)));
                stored_endpoints_[ep.address] = ep.port;
            }
        }
    });
}

}
}
