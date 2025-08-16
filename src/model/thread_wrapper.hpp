#pragma once

namespace p2ptransfer {

/// \brief Thread execution manager to detach applications on other thread.
class ThreadWrapper {
public:
    using Functor = std::function<void()>;

    virtual ~ThreadWrapper() = default;

    /// Returns true if thread is already executing a task.
    virtual bool is_running() const noexcept = 0;
    /// Executes given task on other thread. Check is_running before execute.
    /*! \throws std::logic_error if attached while running */
    virtual void execute(Functor&& func) = 0;
    /// Joins thread if launched before.
    virtual void try_join() noexcept = 0;
};

}
