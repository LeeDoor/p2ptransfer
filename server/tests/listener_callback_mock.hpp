#pragma once
#include "listener_callback.hpp"

namespace general {
namespace server {
namespace presenter {
namespace test {

class ListenerCallbackMock : public ListenerCallback { 
public:
    MOCK_METHOD(bool, verify_file, (const Filename& filename, Filesize filesize), (override));
};

}
}
}
}
