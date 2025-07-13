#include "socket_manager_impl.hpp"
#include "thread_wrapper_impl.hpp"
#include "socket_manager_impl_builder.hpp"

namespace general {
namespace socket_manager {
namespace test {

struct SocketManagerWrapper {
    std::shared_ptr<net::io_context> context_;
    std::shared_ptr<SocketManagerTcp> socket_;

    SocketManagerWrapper(bool is_listening) :
        context_{std::make_shared<net::io_context>()},
        socket_{},
        thread_{}
    {
        net::co_spawn(*context_, connect(is_listening), expect_nothrow);
    }
    void detach() {
        thread_.execute([this] {
            context_->run();
        });
    }
    void join() {
        thread_.join();
    }

private:
    thread_wrapper::ThreadWrapperImpl thread_;

    net::awaitable<void> connect(bool is_listening) {
        if(is_listening) {
            co_return co_await connect_server();
        } else {
            co_return co_await connect_client();
        }
    }
    net::awaitable<void> connect_server() {
        SocketManagerImplBuilder builder{context_};
        socket_ = std::static_pointer_cast<SocketManagerTcp>(co_await builder.tcp_listening_at(TEST_PORT));
    }
    net::awaitable<void> connect_client() {
        SocketManagerImplBuilder builder{context_};
        socket_ = std::static_pointer_cast<SocketManagerTcp>(co_await builder.tcp_connecting_to(TEST_LOCADDR, TEST_PORT));
    }

    static void expect_nothrow(std::exception_ptr exptr) {
        EXPECT_FALSE(exptr);
    }

};

class SocketManagerFixture : public ::testing::Test {
protected:
    SocketManagerFixture() :
        server_{true},
        client_{false}
    {
        server_.detach();
        client_.detach();
        server_.join();
        client_.join();
    }

    SocketManagerWrapper server_;
    SocketManagerWrapper client_;
};

TEST_F(SocketManagerFixture, connectionEstablishedByDefault) {
    SocketManager::Endpoint server_ep { TEST_LOCADDR, TEST_PORT };
    EXPECT_EQ(server_.socket_->get_local_endpoint(), server_ep);
    EXPECT_EQ(client_.socket_->get_remote_endpoint(), server_ep);
    EXPECT_EQ(server_.socket_->get_remote_endpoint().address, TEST_LOCADDR);
    EXPECT_EQ(client_.socket_->get_local_endpoint().address, TEST_LOCADDR);
}

TEST_F(SocketManagerFixture, clientWritesString_singleBufferUsed) {
    // client_.socket_->send_response("aboba"
}

}
}
}
