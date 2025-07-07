#include "thread_wrapper_impl.hpp"
class ThreadWrapperFixture : public ::testing::Test {
protected:
    ThreadWrapperFixture() :
        thread_wrapper_(std::make_shared<ThreadWrapperImpl>())
    {

    }

    std::shared_ptr<ThreadWrapperImpl> thread_wrapper_;
};

TEST_F(ThreadWrapperFixture, success) {
    ASSERT_EQ(true, false);
}
