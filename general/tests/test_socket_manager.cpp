#include "socket_manager_impl.hpp"
#include "socket_manager_impl_builder.hpp"

namespace general {
namespace socket_manager {
namespace test {

class SocketManagerFixture : public ::testing::Test {
protected:
    SocketManagerFixture() :
        server_ctx{std::make_shared<net::io_context>()},
        client_ctx{std::make_shared<net::io_context>()}
    {
        net::co_spawn(*server_ctx, connect_server(), expect_nothrow);
        net::co_spawn(*client_ctx, connect_client(), expect_nothrow);
        server_ctx->run(); client_ctx->run();
    }
    
    net::awaitable<void> connect_server() {
        SocketManagerImplBuilder builder{server_ctx};
        server_side = std::static_pointer_cast<SocketManagerTcp>(co_await builder.tcp_listening_at(TEST_PORT));
    }
    net::awaitable<void> connect_client() {
        SocketManagerImplBuilder builder{client_ctx};
        client_side = std::static_pointer_cast<SocketManagerTcp>(co_await builder.tcp_connecting_to("127.0.0.1", TEST_PORT));
    }

    static void expect_nothrow(std::exception_ptr exptr) {
        EXPECT_FALSE(exptr);
    }

    std::shared_ptr<SocketManagerTcp> server_side;
    std::shared_ptr<net::io_context> server_ctx;
    std::shared_ptr<SocketManagerTcp> client_side;
    std::shared_ptr<net::io_context> client_ctx;
};

TEST_F(SocketManagerFixture, success) {
    SocketManager::Endpoint server_ep { "127.0.0.1", TEST_PORT };
    EXPECT_EQ(server_side->get_local_endpoint(), server_ep);
    EXPECT_EQ(client_side->get_remote_endpoint(), server_ep);
    EXPECT_EQ(server_side->get_remote_endpoint().address, "localhost");
    EXPECT_EQ(client_side->get_local_endpoint().address, "localhost");
}

}
}
}
