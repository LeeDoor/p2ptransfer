#pragma once
#include "request_structures.hpp"

namespace general {

class FilesizeFormatter {
public:
    static std::string to_string(Filesize filesize) {
        if(filesize < (1 << 10)) {
            return std::to_string(filesize) + "B";
        }
        double dsize = filesize;
        if(dsize < (1 << 20)) {
            return std::format("{:.1f}", dsize / (1 << 10)) + "KB";
        }
        if(dsize < (1 << 30)) {
            return std::format("{:.1f}", dsize / (1 << 20)) + "MB";
        }
        if(dsize < (1ll << 40)) {
            return std::format("{:.1f}", dsize / (1 << 30)) + "GB";
        }
        return std::format("{:.2f}", dsize / (1ll << 40)) + "TB";
    }
};

}
