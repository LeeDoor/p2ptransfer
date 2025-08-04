#pragma once

namespace general {

class Stoppable {
public:
    virtual ~Stoppable() {
        if(!stopped_)
            std::cerr << "Object not stopped correctly. Check the stack trace.\n";
    }

    void stop() {
        stop_impl();
        stopped_ = true;
    }

protected:
    virtual void stop_impl() = 0;

private:
    bool stopped_ = false;
};

}
