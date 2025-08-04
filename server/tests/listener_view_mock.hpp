#pragma once
#include "listener_view.hpp"

namespace general {
namespace server {
namespace view {
namespace test {

/// Mock implementation for Listener View.
class ListenerViewMock : public ListenerView {
public:
    MOCK_METHOD(void, setup, (), (override));
    MOCK_METHOD(bool, ask_file_verification, (const Filename& filename, Filesize filesize), (override));
    std::weak_ptr<ListenerViewCallback> get_callback() {
        return callback_;
    }
};

}
}
}
}
