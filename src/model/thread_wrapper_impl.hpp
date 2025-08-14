#pragma once
#include "thread_wrapper.hpp"

namespace p2ptransfer {

/*!
* \brief Implementation for \ref ThreadWrapper .
*
* Uses std::jthread to detach applications.
*/
class ThreadWrapperImpl : public ThreadWrapper {
public:
    ThreadWrapperImpl();

    bool is_running() const noexcept override;
    void execute(Functor&& func) override;
    void join() noexcept override;

private:
    void try_join_thread() noexcept;

    bool is_running_;
    std::jthread thread_;
};

}
