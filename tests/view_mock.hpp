#include "server/view.hpp"
#include "connection_status_callback_mock.hpp"
#include <gmock/gmock.h>

class ViewMock : public IView {
public:
    MOCK_METHOD(int, run, (), (override));
    MOCK_METHOD(void, listen, (), (override));
    MOCK_METHOD(void, set_progressbar, (double), (override));
    MOCK_METHOD(void, set_address, (const Address&), (override));
    MOCK_METHOD(Port, get_port, (), (override));
    MOCK_METHOD(bool, verify_file, (SendRequest), (override));

    MOCK_METHOD(void, connection_opened, (const Address&, Port), (override));
    MOCK_METHOD(void, connected, (const Address&, Port), (override));
    MOCK_METHOD(void, connection_aborted, (const Address&, Port), (override));
    MOCK_METHOD(void, file_transfered, (), (override));
    MOCK_METHOD(void, cant_open_socket, (), (override));
};
