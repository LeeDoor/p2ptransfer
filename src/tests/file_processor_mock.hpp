#pragma once
#include "file_processor.hpp"

namespace p2ptransfer {
namespace model {
namespace test {

/// Mock implementation for \ref FileProcessor .
class FileProcessorMock: public FileProcessor {
public:
    MOCK_METHOD(net::awaitable<void>, try_read_file, (), (override));
};

}
}
}
