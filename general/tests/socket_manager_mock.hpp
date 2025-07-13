#pragma once
#include "socket_manager.hpp"

namespace general {
namespace socket_manager {
namespace test {

class SocketManagerMock : public SocketManager {
public:
    MOCK_METHOD(Endpoint, get_remote_endpoint, (), (override));
    MOCK_METHOD(Endpoint, get_local_endpoint, (), (override));
    MOCK_METHOD(net::awaitable<std::string>, read, (), (override));
    MOCK_METHOD(net::awaitable<void>, write, (const std::string& response), (override));
    MOCK_METHOD(net::awaitable<size_t>, read_part_to, (BufferType& buffer, size_t& bytes_remaining), (override));
};

}
}
}
