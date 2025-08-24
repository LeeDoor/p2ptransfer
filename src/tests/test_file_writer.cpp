#include "file_reader_impl.hpp"
#include "file_writer_impl.hpp"
#include "listener_callback_mock.hpp"
#include "network_status_callback_mock.hpp"
#include "socket_manager_mock.hpp"
#include "request_serializer.hpp"

namespace p2ptransfer {
namespace model {
namespace test {

using namespace ::p2ptransfer::test;
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
    void check_file_writing(const Filename& filename, const std::string& filecontent) {
        verify_and_allow_request(filename, filecontent.size());
        verify_incoming_file(filecontent);
        check_file_transfered();
    }
    void verify_and_allow_request(const Filename& filename, size_t filesize) {
        EXPECT_CALL(*socket_, write(
            RequestSerializer::serialize_send_request(filename, filesize)
        )).WillOnce(Return(return_immediately()));
        EXPECT_CALL(*socket_, read_request())
            .WillOnce(Return(return_immediately(
                RequestSerializer::serialize_send_permission(filename)))
            );
    }
    /// \note Verifies only small files fitting in BufferType::size. 
    void verify_incoming_file(const std::string& filecontent) {
        size_t filesize = filecontent.size();
        EXPECT_LE(filesize, SocketManager::BUFFER_SIZE);
        EXPECT_CALL(*socket_, write_part_from(testing::_, filesize))
            .WillRepeatedly([&] (SocketManager::WriteBufferType& buffer, size_t& bytes_remaining) {
                EXPECT_TRUE(std::equal(filecontent.begin(), filecontent.end(), sv_from_write_buffer(buffer).begin()));
                buffer.grab(bytes_remaining);
                bytes_remaining = 0;
                return return_immediately(filecontent.size());
            });
    }
    inline std::string_view sv_from_write_buffer(SocketManager::WriteBufferType& wb) {
        return std::string_view(wb.get_data(), wb.get_data_size());
    }
    void check_file_transfered() {
        EXPECT_CALL(*network_callback_, set_progressbar).Times(AtLeast(1));
    }

    void run_write_file(const Filename& filename) {
        net::io_context context;
        net::co_spawn(context, file_writer_.write_file(filename), [](std::exception_ptr ptr) {
            if(ptr) std::rethrow_exception(ptr);
        });
        context.run();
    }

    std::shared_ptr<SocketManagerMock> socket_;
    std::shared_ptr<presenter::test::NetworkStatusCallbackMock> network_callback_;
    FileWriterImpl file_writer_;
};

class FileCreator {
public:
    FileCreator(const Filename& filename, const std::string& filecontent) 
        : filename_{filename} {
        std::ofstream ofs{filename, std::ios::binary};
        ofs << filecontent;
    }
    FileCreator(const Filename& filename) : FileCreator(filename, "") {}
    FileCreator(const FileCreator& other) = delete;
    FileCreator(FileCreator&& other) = delete;
    FileCreator& operator= (const Filename& other) = delete;
    FileCreator& operator= (Filename&& other) = delete;
    ~FileCreator() {
        std::filesystem::remove(filename_);
    }
private:
    std::string filename_;
};

TEST_F(FileWriterFixture, averageData_successFileProcessing) {
    const std::string filename = "file.txt";
    const std::string filecontent = "short content";
    check_file_writing(filename, filecontent); 
    FileCreator file_creator(filename, filecontent);

    EXPECT_NO_THROW(run_write_file(filename));
}
/*
TEST_F(FileWriterFixture, LFinContent_successFileProcessing) {
    const std::string filename = "file.txt";
    const std::string filecontent = "too many spaces\n\n and LFs\n\n\n\n\n aboba\n";
    immitate_file_writing(filename, filecontent); 

    EXPECT_NO_THROW(run_write_file(filename));

    verify_file_content(filename, filecontent);
}

TEST_F(FileWriterFixture, spacedFilename_successFileProcessing) {
    const std::string filename = "file name with spaces.txt";
    const std::string filecontent = "Text with\r\n\r\nCRLFS aboba\n";
    immitate_file_writing(filename, filecontent); 

    EXPECT_NO_THROW(run_write_file(filename));

    verify_file_content(filename, filecontent);
}

TEST_F(FileWriterFixture, dottedFilename_successFileProcessing) {
    const std::string filename = "...";
    const std::string filecontent = "Text with\r\n\r\nCRLFS aboba\n";
    immitate_file_writing(filename, filecontent); 

    EXPECT_NO_THROW(run_write_file(filename));

    verify_file_content(filename, filecontent);
}

TEST_F(FileWriterFixture, emptyContent_successFileProcessing) {
    const std::string filename = "filename.txt";
    const std::string filecontent = "";
    immitate_file_writing(filename, filecontent); 

    EXPECT_NO_THROW(run_write_file(filename));

    verify_file_content(filename, filecontent);
}
*/
}
}
}
