#pragma once

class FileProcessor {
public:
    virtual net::awaitable<void> try_read_file() = 0;
};
