#pragma once
#include "logger.hpp"

namespace p2ptransfer {

class Stoppable {
public:
    Stoppable() = default;
    Stoppable(const Stoppable& other) = default;
    Stoppable(Stoppable&& other) {
        stopped_ = other.stopped_;
        other.stopped_ = true;
    }
    Stoppable& operator= (const Stoppable& other) = default;
    Stoppable& operator= (Stoppable&& other) = default;
    virtual ~Stoppable() {
        if(!stopped_) {
            Logger::log_stacktrace("Object not stopped correctly");
#ifdef TESTING 
            std::terminate();   
#endif
        }
    }

    void stop() {
        if(stopped_) {
            Logger::log_stacktrace("Object stopped twice");
#ifdef TESTING 
            std::terminate();   
#endif
        }
        stop_impl();
        stopped_ = true;
    }

protected:
    virtual void stop_impl() = 0;

private:
    bool stopped_ = false;
};

}
