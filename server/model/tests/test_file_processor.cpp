#include "file_processor.hpp"
#include "gtest_headers.hpp"
#include "remote_interaction_callback_mock.hpp"
#include "socket_manager_mock.hpp"
#include "request_serializer.hpp"

constexpr size_t TEST_PORT = 8080;
constexpr std::string TEST_ADDRESS = "127.0.0.1";
constexpr std::string TEST_FILENAME = "test_file.txt";
const std::string TEST_FILE_CONTENT = "This is the test file@@@ content??? !!! \n\n";
const size_t TEST_FILESIZE = TEST_FILE_CONTENT.size();

class FileProcessorFixture : public ::testing::Test {
protected:
    FileProcessorFixture() :
        socket_mock(std::make_shared<SocketManagerMock>()),
        callback_mock(std::make_shared<RemoteInteractionCallbackMock>()),
        file_processor(socket_mock)    
    {
        file_processor.set_callback(callback_mock);
    }

    void immitate_send_request(std::string filename, size_t filesize) {
        EXPECT_CALL(*socket_mock, read_request())
            .WillOnce(Return(return_immediately(
                RequestSerializer::serialize_send_request(filename, filesize))));
    }
    void immitate_user_verification(std::string filename, size_t filesize) {
        EXPECT_CALL(*callback_mock, verify_file(filename, filesize))
            .WillOnce(Return(true));
    }
    void check_response_sending(std::string filename) {
        EXPECT_CALL(*socket_mock, send_response(RequestSerializer::serialize_send_permission(filename)))
            .WillOnce(Return(return_immediately()));
    }
    void immitate_file_content_sending(std::string file_content) {
        size_t filesize = file_content.size();
        size_t bytes_remaining = filesize;
        ASSERT_LE(filesize, std::tuple_size<ISocketManager::BufferType>::value);
        EXPECT_CALL(*socket_mock, read_file_part_to(testing::_, bytes_remaining))
            .WillOnce(([=](ISocketManager::BufferType& buffer, size_t& bytes_remaining) {
                std::copy(file_content.begin(), file_content.end(), buffer.begin());
                bytes_remaining = 0;
                return return_immediately(filesize);
            }));    
    }
    void check_progressbar_callbacks() {
        EXPECT_CALL(*callback_mock, set_progressbar(::testing::_))
            .Times(::testing::AtLeast(1));
    }
    void check_file_transfered_callback() {
        EXPECT_CALL(*callback_mock, file_transfered());
    }
    void run_read_file() {
        net::io_context context;
        net::co_spawn(context, file_processor.try_read_file(), [](std::exception_ptr ptr) {
            if(ptr) std::rethrow_exception(ptr);
        });
        context.run();
    }
    void verify_file_content(std::string filename, std::string file_content) {
        size_t filesize = file_content.size();
        EXPECT_EQ(std::filesystem::file_size("READED_" + filename), filesize);
        std::ifstream ifstream("READED_" + filename);
        ASSERT_TRUE(ifstream.is_open());
        std::stringstream ss;
        ss << ifstream.rdbuf();
        EXPECT_EQ(ss.str(), file_content);
    }

    std::shared_ptr<SocketManagerMock> socket_mock;
    std::shared_ptr<RemoteInteractionCallbackMock> callback_mock;
    FileProcessor file_processor;
};

TEST_F(FileProcessorFixture, process_file_successfully) {
    const std::list<std::string> filenames = { "file.txt", "with space.txt", "noextention", "..." };
    const std::list<std::string> filecontents = { "some short content!\n", "", "terminated\n\n   aboba" };
    for(const std::string& filename : filenames) {
        for(const std::string& file_content : filecontents) {
            std::cerr << filename << ": " << file_content << std::endl;
            size_t filesize = file_content.size();
            immitate_send_request(filename, filesize);
            immitate_user_verification(filename, filesize);
            check_response_sending(filename);
            immitate_file_content_sending(file_content);
            check_progressbar_callbacks();
            check_file_transfered_callback();
            EXPECT_NO_THROW(run_read_file());
            verify_file_content(filename, file_content);
        }
    }
}
TEST_F(FileProcessorFixture, invalidSendRequest_should_abortConnection) {

}
