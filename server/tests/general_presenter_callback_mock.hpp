#pragma once
#include "address.hpp"
#include "port.hpp"
#include "general_presenter_callback.hpp"

namespace general {
namespace server {
namespace presenter {
namespace test {

class GeneralPresenterCallbackMock : public GeneralPresenterCallback { 
public:
    MOCK_METHOD(void, connected, (const Address& address, Port port), (override));
    MOCK_METHOD(void, file_transfered, (), (override));
    MOCK_METHOD(void, connection_aborted, (const Address& address, Port port), (override));
    MOCK_METHOD(void, cant_open_socket, (), (override));
    MOCK_METHOD(void, set_progressbar, (double present), (override));
    // MOCK_METHOD(bool, verify_file, (const Filename& filename, Filesize filesize), (override));
};

}
}
}
}
