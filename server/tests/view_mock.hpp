#pragma once
#include "server_view.hpp"

namespace general {
namespace server {
namespace view {
namespace test {

/// Mock implementation for View.
class ViewMock : public ServerView {
public:
    MOCK_METHOD(int, run, (), (override));
    MOCK_METHOD(void, stop, (), (override));
    MOCK_METHOD(void, show_address, (const Address& address), (override));
    MOCK_METHOD(void, show_connected, (const Address& address, Port port), (override));
    MOCK_METHOD(void, show_socket_error, (), (override));
    MOCK_METHOD(void, update_progressbar_status, (double persent), (override));
    MOCK_METHOD(bool, ask_file_verification, (const Filename& filename, Filesize filesize), (override));
    MOCK_METHOD(void, show_file_success, (), (override));
    MOCK_METHOD(void, show_connection_aborted, (const Address& address, Port port), (override));
    std::weak_ptr<ServerViewCallback> get_callback() {
        return callback_;
    }
};

}
}
}
}
