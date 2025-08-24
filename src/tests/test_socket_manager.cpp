#include "socket_manager_impl.hpp"
#include "logger.hpp"
#include "socket_manager_impl_builder.hpp"
#include "thread_wrapper_impl.hpp"

namespace p2ptransfer {
namespace test {

template<typename Func>
void spawn_at(Func&& func, net::io_context& context) {
    net::co_spawn(context, std::move(func), [](auto exptr) {
        if(exptr) std::rethrow_exception(exptr);
    });
}

template<typename Ret, typename Func>
[[nodiscard]] Ret spawn_get(Func&& func, net::io_context& context) {
    Ret result;
    net::co_spawn(context, std::move(func), [&result](auto exptr, Ret val) {
        if(exptr) std::rethrow_exception(exptr);
        result = std::move(val);
    });
    context.run();
    context.restart();
    return result;
}

class SocketManagerTest : public SocketManagerTcp {
public:
    SocketManagerTest() :
        SocketManagerTest{std::make_shared<net::io_context>()}{}

    void make_server() {
        spawn_yourself(listen_connection_at(TEST_PORT));
    }
    void make_client() {
        spawn_yourself(connect_to(TEST_LOCADDR, TEST_PORT));
    }

    void detach() {
        thread_.execute([this] {
            context_->run();
            context_->restart();
        });
    }

    void join() {
        thread_.try_join();
    }

    void run() {
        context_->run();
        context_->restart();
    }

    template<typename Func>
    void spawn_yourself(Func&& func) {
        spawn_at(std::move(func), *context_);
    }
    template<typename Ret, typename Func>
    [[nodiscard]] Ret spawn_yourself_get(Func&& func) {
        return spawn_get<Ret>(std::move(func), *context_);
    }
    net::awaitable<std::string> read_bytes(size_t bytes) {
        std::stringstream ss;
        BufferType buffer;
        size_t bytes_remaining = bytes;
        while(bytes_remaining) {
            size_t bytes_read = co_await read_part_to(buffer, bytes_remaining);
            std::string data(buffer.data(), bytes_read);
            ss << data;
        }
        co_return ss.str();
    }
    void disconnect() {
        socket_->close();
    }

private:
    SocketManagerTest(std::shared_ptr<net::io_context> context) :
        SocketManagerTcp{context},
        context_{context},
        thread_{}
    {}

    std::shared_ptr<net::io_context> context_;
    ThreadWrapperImpl thread_;
};

class SocketManagerFixture : public ::testing::Test {
protected:
    SocketManagerFixture() :
        server_{},
        client_{}
    {
        server_.make_server();
        client_.make_client();
        server_.detach();
        while(!server_.connected());
        client_.run();
        server_.join();
    }

    static constexpr size_t get_buffer_size() {
        return std::tuple_size<SocketManager::BufferType>::value;
    }

    SocketManagerTest server_;
    SocketManagerTest client_;
};

class SocketManagerBuilderFixture : public ::testing::Test {
protected:
    SocketManagerBuilderFixture() :
        context{std::make_shared<net::io_context>()},
        builder{context}
    {}
    std::shared_ptr<net::io_context> context;
    SocketManagerImplBuilder builder;
};

TEST_F(SocketManagerBuilderFixture, connectingToListenedPort) {
    std::shared_ptr<SocketManager> server, client;

    std::jthread server_thread{[&] {
        auto second_context = std::make_shared<net::io_context>();
        server = spawn_get<std::shared_ptr<SocketManager>>(builder.tcp_listening_at(TEST_PORT), *second_context);
    }};
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    client = spawn_get<std::shared_ptr<SocketManager>>(builder.tcp_connecting_to(TEST_LOCADDR, TEST_PORT), *context);
    server_thread.join();

    auto& r = *server;
    EXPECT_EQ(typeid(r), typeid(SocketManagerTcp));
    EXPECT_TRUE(server->connected());
    EXPECT_TRUE(client->connected());
}

TEST_F(SocketManagerBuilderFixture, connectingToClosedPort_throwsRuntimeError) {
    // TEST_PORT should not be opened ATM
    EXPECT_THROW({
        auto client = spawn_get<std::shared_ptr<SocketManager>>(builder.tcp_connecting_to(TEST_LOCADDR, TEST_PORT), *context);
    }, std::runtime_error);
}

TEST_F(SocketManagerBuilderFixture, connectingToForbiddenPort_throwsRuntimeError) {
    EXPECT_THROW({
        auto client = spawn_get<std::shared_ptr<SocketManager>>(builder.tcp_connecting_to(TEST_LOCADDR, 1), *context);
    }, std::runtime_error);
}

TEST_F(SocketManagerBuilderFixture, abortingWhileReading_throwsRuntimeError) {
    std::shared_ptr<SocketManager> server, client;
    std::jthread server_thread{[&] {
        auto second_context = std::make_shared<net::io_context>();
        server = spawn_get<std::shared_ptr<SocketManager>>(builder.tcp_listening_at(TEST_PORT), *second_context);
    }};
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    client = spawn_get<std::shared_ptr<SocketManager>>(builder.tcp_connecting_to(TEST_LOCADDR, TEST_PORT), *context);
    server_thread.join();

    bool throw_catched = false;
    server_thread = std::jthread{[&] {
        try {
        std::ignore = spawn_get<std::string>(server->read_request(), *context);
        } catch (const std::runtime_error& ex) {
            throw_catched = true;
        }
    }};
    EXPECT_FALSE(throw_catched);
    client.reset();
    server_thread.join();

    EXPECT_TRUE(throw_catched);
}

TEST_F(SocketManagerFixture, afterConstructor_bothConnected) {
    EXPECT_TRUE(server_.connected());
    EXPECT_TRUE(client_.connected());
}

TEST_F(SocketManagerFixture, connectionEstablishedByDefault) {
    SocketManager::Endpoint server_ep { TEST_LOCADDR, TEST_PORT };
    EXPECT_EQ(server_.get_local_endpoint(), server_ep);
    EXPECT_EQ(client_.get_remote_endpoint(), server_ep);
    EXPECT_EQ(server_.get_remote_endpoint().address, TEST_LOCADDR);
    EXPECT_EQ(client_.get_local_endpoint().address, TEST_LOCADDR);
}

TEST_F(SocketManagerFixture, clientWritesString) {
    client_.spawn_yourself(client_.write("aboba\n\n"));
    client_.run();

    auto result = server_.spawn_yourself_get<std::string>(server_.read_request());

    EXPECT_EQ(result, "aboba\n\n");
}

TEST_F(SocketManagerFixture, multipleSequentialExchangeAllowed) {
    for(int i = 0; i < 20; ++i) {
        const std::string request = "aboba" + std::to_string(i) + "\n\n";
        client_.spawn_yourself(client_.write(request));
        client_.run();

        auto result = server_.spawn_yourself_get<std::string>(server_.read_request());

        EXPECT_EQ(result, request);
    }   
}

TEST_F(SocketManagerFixture, sendingTwoRequestsAtOnce_shouldReadOneByOne) {
    const std::string request = "aboba\n\n";
    client_.spawn_yourself(client_.write(request + request));
    client_.run();

    auto result1 = server_.spawn_yourself_get<std::string>(server_.read_request());
    auto result2 = server_.spawn_yourself_get<std::string>(server_.read_request());

    EXPECT_EQ(result1, request);
    EXPECT_EQ(result2, request);
}

TEST_F(SocketManagerFixture, readAndWriteSimultaniously) {
    const std::string server_to_client = "Some server to client text to read\n\n";
    const std::string client_to_server = "Some client to server text to read\n\n";
    client_.spawn_yourself(client_.write(client_to_server));
    server_.spawn_yourself(server_.write(server_to_client));
    client_.run();
    server_.run();

    auto server_read = server_.spawn_yourself_get<std::string>(server_.read_request());
    auto client_read = client_.spawn_yourself_get<std::string>(client_.read_request());

    EXPECT_EQ(server_read, client_to_server);
    EXPECT_EQ(client_read, server_to_client);
}

TEST_F(SocketManagerFixture, sendMoreThanRestriction_closeConnection) {
    std::stringstream ss;
    for(size_t i = 0; i < get_buffer_size() * 2; ++i) ss << "TrashText";
    ss << "\n\n";
    client_.spawn_yourself(client_.write(ss.str()));
    client_.run();

    EXPECT_THROW({
        auto server_read = server_.spawn_yourself_get<std::string>(server_.read_request());
    }, std::runtime_error);
}

TEST_F(SocketManagerFixture, readingFilePart_lessThanOneBuffer) {
    constexpr size_t less_than_buffer = get_buffer_size() / 2;
    std::string file_immitation(less_than_buffer, 'a');
    client_.spawn_yourself(client_.write(file_immitation));
    client_.run();

    SocketManager::BufferType buffer; size_t bytes_remaining = less_than_buffer;
    size_t bytes = server_.spawn_yourself_get<size_t>(server_.read_part_to(buffer, bytes_remaining));

    EXPECT_LE(bytes, less_than_buffer);
    EXPECT_GE(bytes_remaining, 0);
    EXPECT_TRUE(std::equal(buffer.begin(), buffer.begin() + bytes, file_immitation.begin()));
}

TEST_F(SocketManagerFixture, readingFile_BuffersInCycle) {
    constexpr size_t many_buffer_sizes = get_buffer_size() * 10;
    std::string file_immitation(many_buffer_sizes, 'a');
    client_.spawn_yourself(client_.write(file_immitation));
    client_.run();

    SocketManager::BufferType buffer; 
    size_t bytes_remaining = many_buffer_sizes;
    size_t bytes, bytes_gone = 0;
    do {
        size_t old_bytes_remaining = bytes_remaining;
        bytes = server_.spawn_yourself_get<size_t>(server_.read_part_to(buffer, bytes_remaining));
        EXPECT_EQ(old_bytes_remaining - bytes_remaining, bytes);
        EXPECT_LE(bytes, get_buffer_size());
        EXPECT_TRUE(std::equal(buffer.begin(), buffer.begin() + bytes, file_immitation.begin() + bytes_gone));
        bytes_gone += bytes;
    } while(bytes);

    ASSERT_EQ(bytes_gone, many_buffer_sizes);
    ASSERT_EQ(bytes_remaining, 0);
}

TEST_F(SocketManagerFixture, readingFilePartThrows_shouldThrowRuntimeError) {
    constexpr size_t many_buffer_sizes = get_buffer_size() * 10;
    std::string file_immitation(many_buffer_sizes, 'a');
    client_.disconnect();

    SocketManager::BufferType buffer; 
    size_t bytes_remaining = many_buffer_sizes;
    EXPECT_THROW(
        {
            std::ignore = server_.spawn_yourself_get<size_t>(server_.read_part_to(buffer, bytes_remaining));
        }, std::runtime_error);
}

TEST_F(SocketManagerFixture, writingFilePart_lessThanOneBuffer) {
    constexpr size_t less_than_buffer = get_buffer_size() / 2;
    std::string file_immitation(less_than_buffer, 'a');
    SocketManager::BufferType buffer; size_t bytes_remaining = less_than_buffer;
    std::copy_n(file_immitation.begin(), less_than_buffer, buffer.begin());
    size_t bytes = server_.spawn_yourself_get<size_t>(server_.write_part_from(buffer, bytes_remaining));
    std::string gathered_file = client_.spawn_yourself_get<std::string>(client_.read_bytes(bytes));

    EXPECT_LE(bytes, less_than_buffer);
    EXPECT_GE(bytes_remaining, 0);
    EXPECT_EQ(gathered_file.substr(0, bytes), file_immitation.substr(0, bytes));
}

TEST_F(SocketManagerFixture, writingFile_BuffersInCycle) {
    constexpr size_t many_buffer_sizes = get_buffer_size() * 10;
    std::string file_immitation(many_buffer_sizes, 'a');

    SocketManager::BufferType buffer; 
    size_t bytes_remaining = many_buffer_sizes;
    size_t bytes, bytes_gone = 0;
    do {
        size_t old_bytes_remaining = bytes_remaining;
        std::copy_n(file_immitation.begin(), buffer.size(), buffer.begin());
        bytes = server_.spawn_yourself_get<size_t>(server_.write_part_from(buffer, bytes_remaining));
        EXPECT_EQ(old_bytes_remaining - bytes_remaining, bytes);
        EXPECT_LE(bytes, get_buffer_size());
        std::string read_data = client_.spawn_yourself_get<std::string>(client_.read_bytes(bytes));
        EXPECT_TRUE(std::equal(buffer.begin(), buffer.begin() + bytes, read_data.begin()));
        bytes_gone += bytes;
    } while(bytes);

    ASSERT_EQ(bytes_gone, many_buffer_sizes);
    ASSERT_EQ(bytes_remaining, 0);
}

}
}
