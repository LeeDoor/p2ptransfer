#include "socket_manager_impl.hpp"
#include "thread_wrapper_impl.hpp"

namespace general {
namespace socket_manager {
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
        thread_.join();
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

private:
    SocketManagerTest(std::shared_ptr<net::io_context> context) :
        SocketManagerTcp{*context},
        context_{context},
        thread_{}
    {}

    std::shared_ptr<net::io_context> context_;
    thread_wrapper::ThreadWrapperImpl thread_;
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

}
}
}
