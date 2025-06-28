#include "gtest_headers.hpp"
#include "socket_manager_impl.hpp"

class SocketManagerFixture : public ::testing::Test {
protected:
    SocketManagerFixture() 
    {}
    std::shared_ptr<SocketManagerTcp> socket_manager_;
};

TEST_F(SocketManagerFixture, success) {

}
