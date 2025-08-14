#pragma once
#include "general_view.hpp"

namespace Ui {
class GeneralViewGUI;
}

namespace p2ptransfer {
namespace server {
namespace view {

/// \ref View implementation for GUI using Qt.
class GeneralViewGUI : public QMainWindow, public GeneralView {
    Q_OBJECT
public:
    /*! 
     * \param application keeps std::weak_ptr to QApplication for loop running.
     * Original std::shared_ptr to QApplication should 
     * be accessible for an entire ViewGUI lifetime.
    */
    explicit GeneralViewGUI(std::shared_ptr<QApplication> application);

    /// Required to filter drop events.
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

    int run() override;

    void show_address(const Address& ipaddress) override;
    void update_progressbar_status(double persent) override;
    void show_connected(const Address& address, Port port) override;
    void show_connection_aborted(const Address& address, Port port) override;
    void show_file_success() override;
    void show_socket_error() override;

    std::shared_ptr<Ui::GeneralViewGUI> get_ui();
    /// Attach \ref function execution to the \ref run() 's thread. Qt requirement.
    template<typename Func>
    void run_sync(Func&& function) {
        QMetaObject::invokeMethod(this, std::move(function), Qt::BlockingQueuedConnection);
    }

public slots:
    /// Slot for Action button pressed event
    void action_button_clicked();
    /// Slot for select_file button pressed event
    void select_file_button_clicked();
    /// Slot for changing tabs to detect current action
    void action_changed(int index);

signals:
    void listening(Port port);

private:
    void stop_impl() override;

    enum Action { Listen, Transfer };
    Action action() const;
    bool is_listen() const;
    bool is_transfer() const;
    void prepare_ui();

    /// Reads port from user input.
    /*! \throws std::runtime_error if port from label is not an integer or too big */
    Port get_port() const;
    /// Reads IP address from transfer input line.
    /*! \throws std::logic_error if called while from the opened listening bar */
    /*! \throws std::runtime_error if does not match a port regex */
    Address get_address() const;

    void set_file_if_accessible(QString filepath);

    /// Event for closing window.
    void closeEvent(QCloseEvent* e) override;

    /// disables the button and port input.
    /// Enable with \ref enable_ui()
    void disable_ui();
    /// Enables the button and port input.
    /// Disable with \ref disable_ui()
    void enable_ui();

    std::shared_ptr<Ui::GeneralViewGUI> ui_;
    std::weak_ptr<QApplication> application_;
    Action action_;
    QString selected_file_;
};

}
}
}
