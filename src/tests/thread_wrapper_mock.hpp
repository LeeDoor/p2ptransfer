#pragma once
#include "thread_wrapper.hpp"

namespace general {
namespace test {

/// Mock Implementation for \ref ThreadWrapper
class ThreadWrapperMock : public ThreadWrapper {
public:
    MOCK_METHOD(bool, is_running, (), (const, noexcept, override));
    MOCK_METHOD(void, join, (), (noexcept, override));
    MOCK_METHOD(void, mock_execute, (), ());

protected:
    /// Executes given function synchronically
    void execute(Functor&& functor) override {
        mock_execute();
        functor();
    }
};

}
}
