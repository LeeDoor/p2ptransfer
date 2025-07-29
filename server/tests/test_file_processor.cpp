#include "file_processor_impl.hpp"
#include "general_presenter_callback_mock.hpp"
#include "socket_manager_mock.hpp"
#include "request_serializer.hpp"

namespace general {
namespace server {
namespace model {
namespace test {

using namespace ::general::test;

class FileProcessorFixture : public ::testing::Test {
protected:
    using BufferType = SocketManager::BufferType;

    FileProcessorFixture() :
        socket_mock(std::make_shared<SocketManagerMock>()),
        callback_mock(std::make_shared<presenter::test::GeneralPresenterCallbackMock>()),
        file_processor(socket_mock)    
    {
        file_processor.set_callback(callback_mock);
    }

    void success_lifecycle(const std::string& filename, const std::string& file_content) {
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
    void immitate_user_confirmation([[maybe_unused]] const std::string& filename, [[maybe_unused]] size_t filesize, [[maybe_unused]] bool is_confirming) {
        #if 0
        EXPECT_CALL(*callback_mock, verify_file(filename, filesize))
            .WillOnce(Return(is_confirming));
        #endif
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
        EXPECT_CALL(*callback_mock, connection_aborted(TEST_LOCADDR, TEST_PORT));
    }

    void assert_filesize(size_t filesize) {
        ASSERT_LE(filesize, get_buffer_size());
    }   
    constexpr size_t get_buffer_size() {
        return std::tuple_size<BufferType>::value;
    }

    std::shared_ptr<SocketManagerMock> socket_mock;
    std::shared_ptr<presenter::test::GeneralPresenterCallbackMock> callback_mock;
    FileProcessorImpl file_processor;
};

TEST_F(FileProcessorFixture, averageData_successFileProcessing) {
    const std::string filename = "file.txt";
    const std::string filecontent = "short content";
    success_lifecycle(filename, filecontent); 

    EXPECT_NO_THROW(run_read_file());

    verify_file_content(filename, filecontent);
}

TEST_F(FileProcessorFixture, LFinContent_successFileProcessing) {
    const std::string filename = "file.txt";
    const std::string filecontent = "too many spaces\n\n and LFs\n\n\n\n\n aboba\n";
    success_lifecycle(filename, filecontent); 

    EXPECT_NO_THROW(run_read_file());

    verify_file_content(filename, filecontent);
}

TEST_F(FileProcessorFixture, spacedFilename_successFileProcessing) {
    const std::string filename = "file name with spaces.txt";
    const std::string filecontent = "Text with\r\n\r\nCRLFS aboba\n";
    success_lifecycle(filename, filecontent); 

    EXPECT_NO_THROW(run_read_file());

    verify_file_content(filename, filecontent);
}

TEST_F(FileProcessorFixture, dottedFilename_successFileProcessing) {
    const std::string filename = "...";
    const std::string filecontent = "Text with\r\n\r\nCRLFS aboba\n";
    success_lifecycle(filename, filecontent); 

    EXPECT_NO_THROW(run_read_file());

    verify_file_content(filename, filecontent);
}

TEST_F(FileProcessorFixture, emptyContent_successFileProcessing) {
    const std::string filename = "filename.txt";
    const std::string filecontent = "";
    success_lifecycle(filename, filecontent); 

    EXPECT_NO_THROW(run_read_file());

    verify_file_content(filename, filecontent);
}

TEST_F(FileProcessorFixture, sentTooMuch_readOnlyGivenData) {
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

TEST_F(FileProcessorFixture, contentOutOfBufferSize_successFileProcessing) {
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

TEST_F(FileProcessorFixture, readSendRequestThrowsException_abortRethrow) {
    const std::string filename = "new_file.txt";
    const std::string filecontent = "some content\n";
    EXPECT_CALL(*socket_mock, read_request())
        .WillOnce([=]() -> net::awaitable<std::string> 
                  { throw std::runtime_error("immitating send_request reading error"); });
    check_connection_aborted_callback();

    EXPECT_THROW(run_read_file(), std::runtime_error);

    EXPECT_FALSE(std::filesystem::exists(filename));
}

TEST_F(FileProcessorFixture, invalidRequestGiven_abortRethrow) {
    EXPECT_CALL(*socket_mock, read_request())
        .WillOnce([=]() -> net::awaitable<std::string> 
                  { using namespace std::literals; return return_immediately("INVALID_REQUEST\nFILEisbad\nSIZETOO\n\n"s); });
    check_connection_aborted_callback();

    EXPECT_THROW(run_read_file(), std::runtime_error);
}

TEST_F(FileProcessorFixture, requestFilenameIsDirectory_abortRethrow) {
    const std::string filename = "directory/new_file.txt";
    const std::string filecontent = "some content\n";
    size_t filesize = filecontent.size();
    immitate_send_request(filename, filesize);
    check_connection_aborted_callback();

    EXPECT_THROW(run_read_file(), std::runtime_error);

    EXPECT_FALSE(std::filesystem::exists(filename));
}

TEST_F(FileProcessorFixture, userDeclined_exceptionWithoutFile) {
    const std::string filename = "new_file.txt";
    const std::string filecontent = "some content\n";
    size_t filesize = filecontent.size();
    immitate_send_request(filename, filesize);
    immitate_user_confirmation(filename, filesize, false); 
    check_connection_aborted_callback();

    EXPECT_THROW(run_read_file(), std::runtime_error);

    EXPECT_FALSE(std::filesystem::exists(filename));
}

TEST_F(FileProcessorFixture, sendResponseException_abortRethrow) {
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

TEST_F(FileProcessorFixture, exceptionWhileReadingFile_abortRethrow) {
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
}
