#include "socket_manager_impl.hpp"

namespace general {
namespace socket_manager {
namespace test {

class SocketManagerFixture : public ::testing::Test {
protected:
    SocketManagerFixture() 
    {}
    std::shared_ptr<SocketManagerTcp> socket_manager_;
};

TEST_F(SocketManagerFixture, success) {

}

}
}
}
