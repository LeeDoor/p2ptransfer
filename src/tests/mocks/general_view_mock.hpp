#pragma once
#include "general_view.hpp"

namespace p2ptransfer {
namespace view {
namespace test {

/// Mock implementation for View.
class GeneralViewMock : public GeneralView {
public:
    MOCK_METHOD(int, run, (), (override));
    MOCK_METHOD(void, stop, (), (override));
    MOCK_METHOD(void, show_address, (const Address& address), (override));
    MOCK_METHOD(void, show_connected, (const Address& address, Port port), (override));
    MOCK_METHOD(void, show_socket_error, (std::string reason), (override));
    MOCK_METHOD(void, update_progressbar_status, (double persent, double kbps), (override));
    MOCK_METHOD(void, show_file_success, (), (override));
    MOCK_METHOD(void, show_transfer_failed, (const Address& address, Port port, std::string reason), (override));
};

}
}
}
