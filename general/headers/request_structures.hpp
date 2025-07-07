#pragma once
using SerializedText = std::string;
using Filename = std::string;
using Filesize = size_t;
enum RequestMethod {
    REQUEST,
    PERMISSION,
    FILENAME,
    FILESIZE,
};
struct SendRequest {
    Filename filename;
    Filesize filesize;
};
struct SendResponse {
    Filename filename;
};
