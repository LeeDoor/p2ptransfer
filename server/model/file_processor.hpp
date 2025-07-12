#pragma once

namespace general {
namespace server {
namespace model {

class FileProcessor {
public:
    virtual net::awaitable<void> try_read_file() = 0;

    virtual ~FileProcessor() = default;
};

}
}
}
