#pragma once

#include "file_processor.hpp"
class FileProcessorMock: public FileProcessor {
public:
    MOCK_METHOD(net::awaitable<void>, try_read_file, (), (override));
};
