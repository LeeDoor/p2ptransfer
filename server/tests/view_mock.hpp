#pragma once

#include "view.hpp"
class ViewMock : public View {
public:
    MOCK_METHOD(int, run, (), (override));
    MOCK_METHOD(void, show_address, (const Address& address), (override));
    MOCK_METHOD(void, show_connected, (const Address& address, Port port), (override));
    MOCK_METHOD(void, show_socket_error, (), (override));
    MOCK_METHOD(void, update_progressbar_status, (double persent), (override));
    MOCK_METHOD(bool, ask_file_verification, (const Filename& filename, Filesize filesize), (override));
    MOCK_METHOD(void, show_file_success, (), (override));
    MOCK_METHOD(void, show_connection_aborted, (const Address& address, Port port), (override));
};
