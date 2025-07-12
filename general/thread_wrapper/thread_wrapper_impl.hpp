#pragma once
#include "thread_wrapper.hpp"

namespace general {
namespace thread_wrapper {

class ThreadWrapperImpl : public ThreadWrapper {
public:
    ThreadWrapperImpl();
    bool is_running() override;
    void execute(Functor&& func) override;
    void join() override;

private:
    void try_join_thread();

    bool is_running_;
    std::jthread thread_;
};

}
}
