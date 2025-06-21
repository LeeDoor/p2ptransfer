#pragma once

#include "i_address_gatherer.hpp"
#include "presenter.hpp"
#include "mainwindow.hpp"
#include "qt_headers.hpp"
#include "ui_callback.hpp"
#include "listener.hpp"
class GUIPresenter : public IUICallback, public Presenter {
public:
    GUIPresenter(int& argc, char** argv,
                 std::shared_ptr<IListener> listener,
                 std::shared_ptr<IAddressGatherer> address_gatherer);
    std::shared_ptr<GUIPresenter> shared_from_this();
    void setup();
    int run() override;

    void listen() override;
    void set_progressbar(double persent) override;
    void set_address(const Address& address) override;
    void cant_open_socket() override;

    void connection_opened(const Address& address, Port port) override;
    void connection_aborted(const Address& address, Port port) override;
    void connected(const Address& address, Port port) override;
    void file_transfered() override;
    bool verify_file(const Filename& filename, Filesize filesize) override;
protected:
    void setup_callbacks();
    void setup_ui();
    QString filesize_to_qstring(Filesize filesize);
    QApplication application_;
    std::shared_ptr<MainWindow> window_;
    std::shared_ptr<IListener> listener_;
    std::shared_ptr<IAddressGatherer> address_gatherer_;
};
