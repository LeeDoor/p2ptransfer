#pragma once
#include "address.hpp"
#include "port.hpp"
#include "network_status_callback.hpp"

namespace p2ptransfer {
namespace presenter {
namespace test {

class NetworkStatusCallbackMock : public NetworkStatusCallback { 
public:
    MOCK_METHOD(void, connected, (const Address& address, Port port), (override));
    MOCK_METHOD(void, file_transfered, (), (override));
    MOCK_METHOD(void, connection_aborted, (const Address& address, Port port), (override));
    MOCK_METHOD(void, cant_open_socket, (), (override));
    MOCK_METHOD(void, set_progressbar, (double present), (override));
};

}
}
}
