#pragma once
#include "general_view.hpp"
#include "listener_view_callback.hpp"

namespace general {
namespace server {
namespace view {
namespace test {

/// Mock implementation for View.
class ViewMock : public GeneralView {
public:
    MOCK_METHOD(int, run, (), (override));
    MOCK_METHOD(void, stop, (), (override));
    MOCK_METHOD(void, show_address, (const Address& address), (override));
    MOCK_METHOD(void, show_connected, (const Address& address, Port port), (override));
    MOCK_METHOD(void, show_socket_error, (), (override));
    MOCK_METHOD(void, update_progressbar_status, (double persent), (override));
#if 0
    MOCK_METHOD(bool, ask_file_verification, (const Filename& filename, Filesize filesize), (override));
#endif
    MOCK_METHOD(void, show_file_success, (), (override));
    MOCK_METHOD(void, show_connection_aborted, (const Address& address, Port port), (override));
    std::weak_ptr<ListenerViewCallback> get_callback() {
        return callback_;
    }
};

}
}
}
}
