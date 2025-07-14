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
Ret spawn_get(Func&& func, net::io_context& context) {
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
    Ret spawn_yourself_get(Func&& func) {
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

TEST_F(SocketManagerFixture, clientWritesString_singleBufferUsed) {
    client_.spawn_yourself(client_.write("aboba\n\n"));
    client_.run();

    auto result = server_.spawn_yourself_get<std::string>(server_.read_request());

    EXPECT_EQ(result, "aboba\n\n");
}


}
}
}
