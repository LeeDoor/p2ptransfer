#pragma once

namespace general {
namespace server {
namespace model {

/*!
* \brief Class for reading remote file.
*
* Reads remote file, when file gathering is agreed with user.
* This class is used by the \ref Listener.
*/
class FileProcessor {
public:
    virtual ~FileProcessor() = default;

    virtual net::awaitable<void> try_read_file() = 0;
};

}
}
}
