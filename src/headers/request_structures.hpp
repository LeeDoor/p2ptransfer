#pragma once
namespace p2ptransfer {

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
    Filename filename = "";
    Filesize filesize = 0;
};
struct SendPermission {
    Filename filename;
};

}
