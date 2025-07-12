#pragma once
#include "thread_wrapper.hpp"

namespace general {
namespace thread_wrapper {
namespace test {

class ThreadWrapperMock : public ThreadWrapper {
public:
    MOCK_METHOD(bool, is_running, (), (override));
    MOCK_METHOD(void, join, (), (override));
    MOCK_METHOD(void, mock_execute, (), ());

protected:
    void execute(Functor&& functor) override {
        mock_execute();
        functor();
    }
};

}
}
}
