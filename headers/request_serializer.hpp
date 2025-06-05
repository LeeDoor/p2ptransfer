#pragma once
#include "request_header_names.hpp"

class RequestSerializer {
public:
    static std::optional<std::string> serialize_send_request(const std::string& filename, 
                                                             size_t filesize){
        std::ostringstream os;
        os << REQUEST_HEADER << std::endl;
        if(!serialize_filename_header(os, filename)) { return std::nullopt; }
        if(!serialize_filesize_header(os, filesize)) { return std::nullopt; }
        os << std::endl;
        return os.str();
    }

    static std::optional<std::string> serialize_send_permission(const std::string filename){
        std::ostringstream os;
        os << PERMISSION_HEADER << std::endl;
        if(!serialize_filename_header(os, filename)) { return std::nullopt; }
        os << std::endl;
        return os.str();
    }

private:
    static bool serialize_filename_header(std::ostringstream& os, const std::string& filename){
        os << FILE_HEADER << " " << filename << std::endl;
        return true;
    }

    static bool serialize_filesize_header(std::ostringstream& os, size_t filesize){
        os << SIZE_HEADER << " " << filesize << std::endl;
        return true;
    }
};
