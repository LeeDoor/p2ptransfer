#include "thread_wrapper_impl.hpp"

namespace p2ptransfer {
namespace test {

class ThreadWrapperFixture : public ::testing::Test {
protected:
    ThreadWrapperFixture() :
        thread_wrapper_(std::make_shared<ThreadWrapperImpl>())
    { }
    
    static void sleep_ms(unsigned long long ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    unsigned long long EPSILON = 10;
    std::shared_ptr<ThreadWrapperImpl> thread_wrapper_;
};

TEST_F(ThreadWrapperFixture, notRunning_onStartup) {
    EXPECT_FALSE(thread_wrapper_->is_running());
}

TEST_F(ThreadWrapperFixture, joiningWhenNotRunning_doNothing) {
    EXPECT_NO_THROW(thread_wrapper_->join());
    EXPECT_FALSE(thread_wrapper_->is_running());
}

TEST_F(ThreadWrapperFixture, exec1secSleep_ShouldRunForASecond) {
    auto duration = 100;
    EXPECT_NO_THROW(thread_wrapper_->execute([duration]{
        sleep_ms(duration);
    }));
    // RUNNING ####################################******
    //    0.0ms|                         -e|  stop|   +e|
    EXPECT_TRUE(thread_wrapper_->is_running());
    sleep_ms(duration - EPSILON);
    EXPECT_TRUE(thread_wrapper_->is_running());
    sleep_ms(duration + EPSILON);
    EXPECT_FALSE(thread_wrapper_->is_running());
}

TEST_F(ThreadWrapperFixture, exec5sJoining_shouldContinueOn5s) {
    auto duration = 200;
    EXPECT_NO_THROW(thread_wrapper_->execute([duration]{
        sleep_ms(duration);
    }));
    auto begin = std::chrono::steady_clock::now();
    thread_wrapper_->join();
    auto end = std::chrono::steady_clock::now();
    auto diff = end - begin;
    unsigned long long elapsed_until_joined = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();

    EXPECT_LE(elapsed_until_joined, duration + EPSILON);
    EXPECT_GE(elapsed_until_joined, duration - EPSILON);
}

TEST_F(ThreadWrapperFixture, afterJoin_neverBeingRun) {
    for(auto duration : {100, 5, 2, 70, 20}) {
        thread_wrapper_->execute([duration] {
            sleep_ms(duration);
        });
        EXPECT_TRUE(thread_wrapper_->is_running());
        thread_wrapper_->join();
        EXPECT_FALSE(thread_wrapper_->is_running());
    }
}

TEST_F(ThreadWrapperFixture, executingWhileRunning_throwLogicError) {
    thread_wrapper_->execute([] {
        sleep_ms(200);
    });
    EXPECT_THROW({
        thread_wrapper_->execute([] {
            sleep_ms(100);
        }); 
    }, std::logic_error);
}

TEST_F(ThreadWrapperFixture, executedCommandOnDifferentThread) {
    auto tid = std::this_thread::get_id();
    std::thread::id wrappers_tid = std::this_thread::get_id();

    EXPECT_NO_THROW(thread_wrapper_->execute([&wrappers_tid]{
        wrappers_tid = std::this_thread::get_id();
    }));
    thread_wrapper_->join();

    EXPECT_NE(wrappers_tid, tid);
}

}
}
