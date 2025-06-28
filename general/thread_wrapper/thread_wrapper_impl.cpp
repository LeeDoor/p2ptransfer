#include "thread_wrapper_impl.hpp"    

ThreadWrapperImpl::ThreadWrapperImpl() :
    is_running_(false){}

bool ThreadWrapperImpl::is_running()  {
    return is_running_;
}

void ThreadWrapperImpl::execute(Functor&& func) {
    if(is_running_) 
        throw std::logic_error("Called ThreadWrapperImpl::execute while thread still running."
                               " Check if it is_running() before execution");
    try_join_thread();
    thread_ = std::jthread([func_ = std::move(func), this] {
        is_running_ = true;
        func_();
        is_running_ = false;
    });
}
void ThreadWrapperImpl::try_join_thread() {
    if(thread_.joinable()) {
        thread_.join();
    }
}
