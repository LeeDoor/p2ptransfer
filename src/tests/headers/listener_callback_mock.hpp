#pragma once
#include "listener_callback.hpp"

namespace p2ptransfer {
namespace presenter {
namespace test {

class ListenerCallbackMock : public ListenerCallback { 
public:
    MOCK_METHOD(bool, verify_file, (const Filename& filename, Filesize filesize), (override));
};

}
}
}
