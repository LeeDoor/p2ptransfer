#include "file_reader_impl.hpp"
#include "listener_callback_mock.hpp"
#include "network_status_callback_mock.hpp"
#include "socket_manager_mock.hpp"
#include "request_serializer.hpp"

namespace p2ptransfer {
namespace model {
namespace test {

using namespace ::p2ptransfer::test;
/*! FileWriter --> asks permission --> remote user
*   remote user --> allows/denies --> FileWriter
*   ---- if allowed --- 
*   FileWriter --> partially sends file --> remote user
*/
class FileWriterFixture : public ::testing::Test {
protected:
    using BufferType = SocketManager::BufferType;

    FileWriterFixture() :
        socket_(std::make_shared<SocketManagerMock>()),
        network_callback_(std::make_shared<presenter::test::NetworkStatusCallbackMock>()),
        file_writer_(socket_)    
    {
        file_writer_.set_callback(network_callback_);
    }
    void check_file_reading(const Filename& filename, const std::string& filecontent) {
        verify_and_allow_request(filename, filecontent.size());
        verify_incoming_file(filecontent);
        check_file_transfered();
    }
    void verify_and_allow_request(const Filename& filename, size_t filesize) {
        EXPECT_CALL(*socket_, write(
            RequestSerializer::serialize_send_request(filename, filesize)
        ));
        EXPECT_CALL(*socket_, read_request())
            .WillOnce(Return(return_immediately(
                RequestSerializer::serialize_send_permission(filename)))
            );
    }
    void verify_incoming_file(const std::string& filecontent) {
    }
    void check_file_transfered() {
        EXPECT_CALL(*network_callback_, file_transfered());
    }

    std::shared_ptr<SocketManagerMock> socket_;
    std::shared_ptr<presenter::test::NetworkStatusCallbackMock> network_callback_;
    FileReaderImpl file_writer_;
};

}
}
}
