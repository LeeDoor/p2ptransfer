#pragma once
#include "logger.hpp"

namespace general {

class Stoppable {
public:
    virtual ~Stoppable() {
        if(!stopped_) {
            std::cerr << Logger::ERROR_COLOR << "Object not stopped correctly:" << Logger::CLEAR_COLOR << "\n"
                << boost::stacktrace::stacktrace() << std::endl;
#ifdef TESTING 
            std::terminate();   
#endif
        }
    }

    void stop() {
#ifdef TESTING 
        if(stopped_) {
            std::cerr << "Object stopped twice.\n";
            std::terminate();   
        }
#endif
        stop_impl();
        stopped_ = true;
    }

protected:
    virtual void stop_impl() = 0;

private:
    bool stopped_ = false;
};

}
