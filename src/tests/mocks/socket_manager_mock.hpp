#pragma once
#include "socket_manager.hpp"

namespace p2ptransfer {
namespace test {

/// Mocks the interface of \ref SocketManager.
class SocketManagerMock : public SocketManager {
public:
    MOCK_METHOD(bool, connected, (), (const, noexcept, override));
    MOCK_METHOD(Endpoint, get_remote_endpoint, (), (const, override));
    MOCK_METHOD(Endpoint, get_local_endpoint, (), (const, override));
    MOCK_METHOD(net::awaitable<std::string>, read_request, (), (override));
    MOCK_METHOD(net::awaitable<void>, write, (std::string response), (override));
    MOCK_METHOD(net::awaitable<size_t>, read_part_to, (BufferType& buffer, size_t& bytes_remaining), (override));
    MOCK_METHOD(net::awaitable<size_t>, write_part_from, (WriteBufferType& buffer, size_t& bytes_remaining), (override));
};

}
}
