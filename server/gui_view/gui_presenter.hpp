#pragma once

#include "connection_status_callback.hpp"
#include "file_transfer_callback.hpp"
#include "gather_address_callback.hpp"
#include "mainwindow.hpp"
#include "qt_headers.hpp"
#include "ui_callback.hpp"
#include "listener.hpp"
#include "address_gatherer.hpp"
class GUIPresenter : public IConnectionStatusCallback, 
                public IFileTransferCallback, 
                public IGatherAddressCallback, 
                public IUICallback, 
                public std::enable_shared_from_this<GUIPresenter>{
public:
    GUIPresenter(int& argc, char** argv,
                 std::shared_ptr<IListener> listener,
                 std::shared_ptr<IAddressGatherer> address_gatherer);
    void setup();
    int run();

    void listen() override;
    void set_progressbar(double persent) override;
    void set_address(const Address& address) override;
    void cant_open_socket() override;

    void connection_opened(const Address& address, Port port) override;
    void connection_aborted(const Address& address, Port port) override;
    void connected(const Address& address, Port port) override;
    void file_transfered() override;
    bool verify_file(SendRequest send_request) override;
private:
    void setup_callbacks();
    void setup_ui();
    QString filesize_to_qstring(Filesize filesize);
    QApplication application_;
    std::shared_ptr<MainWindow> window_;
    std::shared_ptr<IListener> listener_;
    std::shared_ptr<IAddressGatherer> address_gatherer_;
};
