#pragma once
#include "address.hpp"
#include "port.hpp"
#include "network_status_callback.hpp"

namespace p2ptransfer {
namespace presenter {
namespace test {

class NetworkStatusCallbackMock : public NetworkStatusCallback { 
public:
    MOCK_METHOD(void, connection_established, (const Address& address, Port port), (override));
    MOCK_METHOD(void, transfer_succeed, (), (override));
    MOCK_METHOD(void, transfer_failed, (const Address& address, Port port), (override));
    MOCK_METHOD(void, cant_open_socket, (), (override));
    MOCK_METHOD(void, set_progressbar, (size_t bytes_remaining, size_t filesize), (override));
};

}
}
}
