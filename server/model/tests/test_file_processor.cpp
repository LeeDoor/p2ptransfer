#include <gtest/gtest.h>
#include "file_processor.hpp"
#include "socket_manager.hpp"

constexpr size_t TEST_PORT = 8080;
#define TEST_ADDRESS "localhost"

TEST(FileProcessor, testingOk) {
    net::io_context io;
    SocketManager socket_manager;
    SockPtr listener, sender;
    std::jthread thread1 ([&]{
        listener = socket_manager.get_connection_sync(TEST_PORT);
        std::cout << "listened\n";
    }); 
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::jthread thread2 ([&]{
        sender = socket_manager.connect_sync(TEST_ADDRESS, TEST_PORT);
        std::cout << "connected\n";
    }); 
    thread1.join(); thread2.join();
    ASSERT_EQ(listener->local_endpoint().port(), TEST_PORT);
    ASSERT_EQ(sender->remote_endpoint().port(), TEST_PORT);
    FileProcessor file_processor(io, std::move(listener));
}
