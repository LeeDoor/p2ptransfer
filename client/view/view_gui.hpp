#pragma once

#include "client_view.hpp"
namespace Ui {
class ViewGUI;
}

namespace general {
namespace client {
namespace view {

/// \ref View implementation for GUI using Qt.
class ViewGUI : QMainWindow, public ClientView {
    Q_OBJECT
public:
    /*! 
     * \param application keeps std::weak_ptr to QApplication for loop running.
     * Original std::shared_ptr to QApplication should 
     * be accessible for an entire ViewGUI lifetime.
    */
    explicit ViewGUI(std::shared_ptr<QApplication> application);
    ~ViewGUI();

    /// Required to filter drop events.
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

    int run() override;
    void stop() override;

    void show_address(Address address) override;
    void connected(Address address, Port port) override;
    void connection_failed(Address address, Port port) override;
    void update_progressbar(double percent) override;
    void file_transfered() override;
    void access_denied() override;
    void connection_aborted() override;

public slots:
    void send_button_clicked();
    void select_file_button_clicked();

private:
    void set_file_if_accessible(QString filepath);
    QString ask_file_explorer();
    QString get_validated_ip();
    int get_validated_port();

    void disable_ui();
    void enable_ui();
    void set_ui_status(bool locked);

    QString ip_port_to_qstring(Address address, Port port);

    QString selected_file_;
    std::weak_ptr<QApplication> application_;
    Ui::ViewGUI *ui;
};

}
}
}
