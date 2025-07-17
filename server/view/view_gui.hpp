#pragma once
#include "view.hpp"

namespace Ui {
class ViewGUI;
}

namespace general {
namespace server {
namespace view {

/// \ref View implementation for GUI using Qt.
class ViewGUI : QMainWindow, public View {
    Q_OBJECT
public:
    /*! 
     * \param application keeps std::weak_ptr to QApplication for loop running.
     * Original std::shared_ptr to QApplication should 
     * be accessible for an entire ViewGUI lifetime.
    */
    explicit ViewGUI(std::shared_ptr<QApplication> application);
    ~ViewGUI();

    int run() override;
    void stop() override;

    void show_address(const Address& ipaddress) override;
    void update_progressbar_status(double persent) override;
    void show_connected(const Address& address, Port port) override;
    void show_connection_aborted(const Address& address, Port port) override;
    void show_file_success() override;
    bool ask_file_verification(const Filename& filename, Filesize filesize) override;
    void show_socket_error() override;
public slots:
    /// Slot for Listen button pressed event.
    void listen_pressed();

private:
    /// Event for closing window.
    void closeEvent(QCloseEvent* e) override;

    /// Reads port from user input.
    Port get_port() const;
    /// disables the button and port input.
    /// Enable with \ref enable_ui()
    void disable_ui();
    /// Enables the button and port input.
    /// Disable with \ref disable_ui()
    void enable_ui();

    /// Attach \ref function execution to the \ref run() 's thread. Qt requirement.
    template<typename Func>
    void run_sync(Func&& function) {
        QMetaObject::invokeMethod(this, std::move(function), Qt::BlockingQueuedConnection);
    }

    Ui::ViewGUI *ui;
    std::weak_ptr<QApplication> application_;
};

}
}
}
