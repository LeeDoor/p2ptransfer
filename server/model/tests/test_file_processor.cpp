#include <gtest/gtest.h>
#include "file_processor.hpp"
#include "socket_manager.hpp"

constexpr size_t TEST_PORT = 8080;
#define TEST_ADDRESS "localhost"

TEST(FileProcessor, testingOk) {
    net::io_context io;
    SocketManager socket_manager;
    SockPtr listener, sender;
    std::jthread thread ([&]{
        listener = socket_manager.get_connection_sync(TEST_PORT);
    }); 
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    sender = socket_manager.connect_sync(TEST_ADDRESS, TEST_PORT);
    thread.join(); 
    ASSERT_EQ(listener->local_endpoint().port(), TEST_PORT);
    ASSERT_EQ(sender->remote_endpoint().port(), TEST_PORT);
    FileProcessor file_processor(io, std::move(listener));
    thread = std::jthread([&]{
        std::string message = "REQUEST\nFILE aboba.txt\nSIZE 1488\n\n";
        net::write(*sender, net::buffer(message));
    });
    co_spawn(io, file_processor.read_remote_file(), [](std::exception_ptr ptr){});
}
