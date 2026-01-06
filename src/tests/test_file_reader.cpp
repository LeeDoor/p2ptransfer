#include "file_reader_impl.hpp"
#include "listener_callback_mock.hpp"
#include "network_status_callback_mock.hpp"
#include "socket_manager_mock.hpp"
#include "request_serializer.hpp"

namespace p2ptransfer {
namespace model {
namespace test {

using namespace ::p2ptransfer::test;

class FileReaderFixture : public ::testing::Test {
protected:
    using BufferType = SocketManager::BufferType;

    FileReaderFixture() :
        socket_mock(std::make_shared<SocketManagerMock>()),
        network_callback(std::make_shared<presenter::test::NetworkStatusCallbackMock>()),
        listener_callback(std::make_shared<presenter::test::ListenerCallbackMock>()),
        file_reader(socket_mock)    
    {
        file_reader.set_callback(network_callback);
        file_reader.set_callback(listener_callback);
    }

    /// Writing file immitation for small contents (below Buffer size. see \ref SocketManager::BufferType)
    void check_file_reading(const std::string& filename, const std::string& file_content) {
        size_t filesize = file_content.size();
        immitate_send_request(filename, filesize);
        immitate_user_confirmation(filename, filesize, true);
        check_response_sending(filename);
        if(filesize) {
            immitate_file_content_sending(file_content);
            check_progressbar_callbacks();
        }
        check_file_transfered_callback();
    }
    void immitate_send_request(const std::string& filename, size_t filesize) {
        EXPECT_CALL(*socket_mock, read_request())
            .WillOnce(Return(return_immediately(
                RequestSerializer::serialize_send_request(filename, filesize))));
    }
    void immitate_user_confirmation(const std::string& filename, size_t filesize, bool is_confirming) {
        EXPECT_CALL(*listener_callback, verify_file(filename, filesize))
            .WillOnce(Return(is_confirming));
    }
    void check_response_sending(const std::string& filename) {
        EXPECT_CALL(*socket_mock, write(RequestSerializer::serialize_send_permission(filename)))
            .WillOnce(Return(return_immediately()));
    }
    void immitate_file_content_sending(const std::string& file_content) {
        size_t filesize = file_content.size();
        assert_filesize(filesize);
        EXPECT_CALL(*socket_mock, read_part_to(testing::_, filesize))
            .WillOnce([=](BufferType& buffer, size_t& bytes_remaining) {
                std::copy(file_content.begin(), file_content.end(), buffer.begin());
                bytes_remaining = 0;
                return return_immediately(filesize);
            });
    }
    void check_progressbar_callbacks() {
        EXPECT_CALL(*network_callback, set_progressbar(::testing::_, ::testing::_))
            .Times(::testing::AtLeast(1));
    }
    void check_file_transfered_callback() {
        EXPECT_CALL(*network_callback, file_transfered());
    }
    void run_read_file() {
        net::io_context context;
        net::co_spawn(context, file_reader.try_read_file(), [](std::exception_ptr ptr) {
            if(ptr) std::rethrow_exception(ptr);
        });
        context.run();
    }
    void verify_file_content(const std::string& filename, const std::string& expected_file_content) {
        size_t filesize = expected_file_content.size();
        EXPECT_EQ(std::filesystem::file_size("READED_" + filename), filesize);
        std::ifstream ifstream("READED_" + filename, std::ios::binary);
        ASSERT_TRUE(ifstream.is_open());
        std::stringstream ss;
        ss << ifstream.rdbuf();
        EXPECT_EQ(ss.str(), expected_file_content);
        ifstream.close();
        std::filesystem::remove("READED_" + filename);
    }
    void check_connection_aborted_callback() {
        EXPECT_CALL(*socket_mock, get_remote_endpoint())
            .WillOnce(Return(SocketManager::Endpoint{TEST_LOCADDR, TEST_PORT}));
        EXPECT_CALL(*network_callback, connection_aborted(TEST_LOCADDR, TEST_PORT));
    }

    void assert_filesize(size_t filesize) {
        ASSERT_LE(filesize, get_buffer_size());
    }   
    constexpr size_t get_buffer_size() {
        return std::tuple_size<BufferType>::value;
    }

    std::shared_ptr<SocketManagerMock> socket_mock;
    std::shared_ptr<presenter::test::NetworkStatusCallbackMock> network_callback;
    std::shared_ptr<presenter::test::ListenerCallbackMock> listener_callback;
    FileReaderImpl file_reader;
};

TEST_F(FileReaderFixture, averageData_successFileProcessing) {
    const std::string filename = "file.txt";
    const std::string filecontent = "short content";
    check_file_reading(filename, filecontent); 

    EXPECT_NO_THROW(run_read_file());

    verify_file_content(filename, filecontent);
}

TEST_F(FileReaderFixture, LFinContent_successFileProcessing) {
    const std::string filename = "file.txt";
    const std::string filecontent = "too many spaces\n\n and LFs\n\n\n\n\n aboba\n";
    check_file_reading(filename, filecontent); 

    EXPECT_NO_THROW(run_read_file());

    verify_file_content(filename, filecontent);
}

TEST_F(FileReaderFixture, spacedFilename_successFileProcessing) {
    const std::string filename = "file name with spaces.txt";
    const std::string filecontent = "Text with\r\n\r\nCRLFS aboba\n";
    check_file_reading(filename, filecontent); 

    EXPECT_NO_THROW(run_read_file());

    verify_file_content(filename, filecontent);
}

TEST_F(FileReaderFixture, dottedFilename_successFileProcessing) {
    const std::string filename = "...";
    const std::string filecontent = "Text with\r\n\r\nCRLFS aboba\n";
    check_file_reading(filename, filecontent); 

    EXPECT_NO_THROW(run_read_file());

    verify_file_content(filename, filecontent);
}

TEST_F(FileReaderFixture, emptyContent_successFileProcessing) {
    const std::string filename = "filename.txt";
    const std::string filecontent = "";
    check_file_reading(filename, filecontent); 

    EXPECT_NO_THROW(run_read_file());

    verify_file_content(filename, filecontent);
}

TEST_F(FileReaderFixture, sentTooMuch_readOnlyGivenData) {
    const std::string filename = "new_file.txt";
    const std::string filecontent = "some content\n";
    size_t filesize = filecontent.size();
    immitate_send_request(filename, filesize);
    immitate_user_confirmation(filename, filesize, true); 
    check_response_sending(filename);
    assert_filesize(filesize * 2);
    EXPECT_CALL(*socket_mock, read_part_to(testing::_, filesize))
        .WillOnce([=](BufferType& buffer, size_t& bytes_remaining) {
            std::copy(filecontent.begin(), filecontent.end(), buffer.begin());
            // copying extra data
            std::copy(filecontent.begin(), filecontent.end(), buffer.begin() + filecontent.size());
            bytes_remaining = 0;
            return return_immediately(filesize);
        });
    check_progressbar_callbacks();
    check_file_transfered_callback();

    EXPECT_NO_THROW(run_read_file());

    verify_file_content(filename, filecontent);
}

TEST_F(FileReaderFixture, contentOutOfBufferSize_successFileProcessing) {
    const std::string filename = "new_file.txt";
    size_t buffer_size = get_buffer_size();
    size_t filesize = buffer_size * 2;
    immitate_send_request(filename, filesize);
    immitate_user_confirmation(filename, filesize, true); 
    check_response_sending(filename);
    testing::Sequence buffer_sequence;
    EXPECT_CALL(*socket_mock, read_part_to(testing::_, filesize))
        .InSequence(buffer_sequence)
        .WillOnce([=](BufferType& buffer, size_t& bytes_remaining) {
            buffer.fill('f');
            bytes_remaining -= buffer.size();
            return return_immediately(buffer.size());
        });
    EXPECT_CALL(*socket_mock, read_part_to(testing::_, buffer_size))
        .InSequence(buffer_sequence)
        .WillOnce([=](BufferType& buffer, size_t& bytes_remaining) {
            buffer.fill('s');
            bytes_remaining = 0;
            return return_immediately(buffer.size());
        });
    check_progressbar_callbacks();
    check_file_transfered_callback();

    EXPECT_NO_THROW(run_read_file());

    std::string file_content(filesize, 'f');
    std::fill(file_content.begin() + buffer_size, file_content.end(), 's');
    verify_file_content(filename, file_content); 
}

TEST_F(FileReaderFixture, readSendRequestThrowsException_abortRethrow) {
    const std::string filename = "new_file.txt";
    const std::string filecontent = "some content\n";
    EXPECT_CALL(*socket_mock, read_request())
        .WillOnce([=]() -> net::awaitable<std::string> 
                  { throw std::runtime_error("immitating send_request reading error"); });
    check_connection_aborted_callback();

    EXPECT_THROW(run_read_file(), std::runtime_error);

    EXPECT_FALSE(std::filesystem::exists(filename));
}

TEST_F(FileReaderFixture, invalidRequestGiven_abortRethrow) {
    EXPECT_CALL(*socket_mock, read_request())
        .WillOnce([=]() -> net::awaitable<std::string> 
                  { using namespace std::literals; return return_immediately("INVALID_REQUEST\nFILEisbad\nSIZETOO\n\n"s); });
    check_connection_aborted_callback();

    EXPECT_THROW(run_read_file(), std::runtime_error);
}

TEST_F(FileReaderFixture, requestFilenameIsDirectory_abortRethrow) {
    const std::string filename = "directory/new_file.txt";
    const std::string filecontent = "some content\n";
    size_t filesize = filecontent.size();
    immitate_send_request(filename, filesize);
    check_connection_aborted_callback();

    EXPECT_THROW(run_read_file(), std::runtime_error);

    EXPECT_FALSE(std::filesystem::exists(filename));
}

TEST_F(FileReaderFixture, userDeclined_exceptionWithoutFile) {
    const std::string filename = "new_file.txt";
    const std::string filecontent = "some content\n";
    size_t filesize = filecontent.size();
    immitate_send_request(filename, filesize);
    immitate_user_confirmation(filename, filesize, false); 
    check_connection_aborted_callback();

    EXPECT_THROW(run_read_file(), std::runtime_error);

    EXPECT_FALSE(std::filesystem::exists(filename));
}

TEST_F(FileReaderFixture, sendResponseException_abortRethrow) {
    const std::string filename = "new_file.txt";
    const std::string filecontent = "some content\n";
    size_t filesize = filecontent.size();
    immitate_send_request(filename, filesize);
    immitate_user_confirmation(filename, filesize, true); 
    EXPECT_CALL(*socket_mock, write(RequestSerializer::serialize_send_permission(filename)))
        .WillOnce([]() -> net::awaitable<void> { 
            throw std::runtime_error("imitating exception while sending permission"); 
        });
    check_connection_aborted_callback();

    EXPECT_THROW(run_read_file(), std::runtime_error);
}

TEST_F(FileReaderFixture, exceptionWhileReadingFile_abortRethrow) {
    const std::string filename = "new_file.txt";
    const std::string filecontent = "some content\n";
    size_t filesize = filecontent.size();
    immitate_send_request(filename, filesize);
    immitate_user_confirmation(filename, filesize, true); 
    check_response_sending(filename);
    ASSERT_LE(filesize, std::tuple_size<BufferType>::value);
    EXPECT_CALL(*socket_mock, read_part_to(testing::_, filesize))
        .WillOnce(::testing::WithoutArgs([=]() 
                  -> net::awaitable<size_t> {
            throw std::runtime_error("immitating reading data exception");
        }));
    check_connection_aborted_callback();

    EXPECT_THROW(run_read_file(), std::runtime_error);
}

}
}
}
