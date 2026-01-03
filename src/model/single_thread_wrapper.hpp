#pragma once
#include "thread_wrapper.hpp"

namespace p2ptransfer {

/// Single-threaded ThreadWrapper
class SingleThreadWrapper : public ThreadWrapper {
public:
    bool is_running() const noexcept override {
        return is_running_;
    }
    void execute(Functor&& func) override {
        is_running_ = true;
        func();
        is_running_ = false;
    }
    void try_join() noexcept override {}

private:
    bool is_running_ = false;
};

}
