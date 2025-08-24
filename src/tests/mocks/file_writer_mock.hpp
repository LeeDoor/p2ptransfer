#pragma once
#include "file_writer.hpp"

namespace p2ptransfer {
namespace model {
namespace test {

class FileWriterMock : public FileWriter {
public:
    MOCK_METHOD(net::awaitable<void>, write_file, (const Filename&), (override));
    FileWriterMock() {
        // EXPECT_CALL(this, write_file).WillRepeatedly(
    }
};

}
}
}
