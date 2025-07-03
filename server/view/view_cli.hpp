#pragma once
#include "view.hpp"

class ViewCLI : public View {
public:
     int run() override;
     void stop() override;
     void show_address(const Address& address) override;
     void show_connected(const Address& address, Port port) override;
     void show_socket_error() override;
     void update_progressbar_status(double persent) override;
     bool ask_file_verification(const Filename& filename, Filesize filesize) override;
     void show_file_success() override;
     void show_connection_aborted(const Address& address, Port port) override;
private:
    bool is_running_ = true;
};
