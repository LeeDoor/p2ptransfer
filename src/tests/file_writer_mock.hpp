#pragma once
#include "file_writer.hpp"

namespace p2ptransfer {
namespace model {
namespace test {

class FileWriterMock : public FileWriter {
public:
    MOCK_METHOD(void, write_file_to, (const Address&, Port, const Filename&), (override));
};

}
}
}
