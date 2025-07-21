#pragma once
#include "client_view.hpp"

namespace general {
namespace client {
namespace view {
namespace test {

class ViewMock : public ClientView {
public:
    MOCK_METHOD(int, run, (), (override));
    MOCK_METHOD(void, stop, (), (override));

    MOCK_METHOD(void, show_address, (Address address), (override));
    MOCK_METHOD(void, connected, (Address address, Port port), (override));
    MOCK_METHOD(void, connection_failed, (Address address, Port port), (override));
    MOCK_METHOD(void, update_progressbar, (double percent), (override));
    MOCK_METHOD(void, file_transfered, (), (override));
    MOCK_METHOD(void, access_denied, (), (override));
    MOCK_METHOD(void, connection_aborted, (), (override));

    std::weak_ptr<ViewCallback> get_callback() {
        return callback_;
    }
};


}
}
}
}
