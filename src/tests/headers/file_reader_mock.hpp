#pragma once
#include "file_reader.hpp"

namespace p2ptransfer {
namespace model {
namespace test {

/// Mock implementation for \ref FileReader .
class FileReaderMock: public FileReader {
public:
    MOCK_METHOD(net::awaitable<void>, try_read_file, (), (override));
};

}
}
}
