#pragma once
#include "gtest_headers.hpp"
#include "socket_manager.hpp"
class SocketManagerMock : public ISocketManager {
public:
    MOCK_METHOD(net::awaitable<void>, establish_connection_async, (Port port), (override));
    MOCK_METHOD(RemoteEndpoint, get_remote_endpoint, (), (override));
    MOCK_METHOD(net::awaitable<std::string>, read_request, (), (override));
    MOCK_METHOD(net::awaitable<void>, send_response, (const std::string& response), (override));
    MOCK_METHOD(net::awaitable<size_t>, read_file_part_to, (BufferType& buffer, size_t& bytes_remaining), (override));
};
