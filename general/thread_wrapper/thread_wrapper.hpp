#pragma once
#include <functional>

namespace general {
namespace thread_wrapper {

class ThreadWrapper {
public:
    using Functor = std::function<void()>;
    virtual bool is_running() = 0;
    virtual void execute(Functor&& func) = 0;
    virtual void join() = 0;

    virtual ~ThreadWrapper() = default;
};

}
}
