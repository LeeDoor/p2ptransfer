#pragma once
#include "general_view.hpp"

namespace p2ptransfer {
namespace view {
namespace test {

/// Mock implementation for View.
class GeneralViewMock : public GeneralView {
public:
    MOCK_METHOD(int, run, (), (override));
    MOCK_METHOD(void, stop_impl, (), (override));
    MOCK_METHOD(void, show_address, (const Address& address), (override));
    MOCK_METHOD(void, show_connected, (const Address& address, Port port), (override));
    MOCK_METHOD(void, show_socket_error, (), (override));
    MOCK_METHOD(void, update_progressbar_status, (double persent), (override));
    MOCK_METHOD(void, show_file_success, (), (override));
    MOCK_METHOD(void, show_connection_aborted, (const Address& address, Port port), (override));
};

}
}
}
