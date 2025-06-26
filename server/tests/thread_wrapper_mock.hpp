#pragma once

#include "thread_wrapper.hpp"
class ThreadWrapperMock : public ThreadWrapper {
public:
    MOCK_METHOD(bool, is_running, (), (override));
    MOCK_METHOD(void, execute, (Functor&& functor), (override));
};
