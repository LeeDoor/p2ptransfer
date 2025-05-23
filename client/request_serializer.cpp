#include "request_serializer.hpp"
#include "request_header_names.hpp"

std::optional<std::string> RequestSerializer::serialize_send_request(const std::string& filename, 
                                                           size_t filesize) {
    std::ostringstream os;
    os << REQUEST_HEADER << std::endl;
    if(!serialize_filename_header(os, filename)) { return std::nullopt; }
    if(!serialize_filesize_header(os, filesize)) { return std::nullopt; }
    return os.str();
}

bool RequestSerializer::serialize_filename_header(std::ostringstream& os, const std::string& filename) {
    os << FILE_HEADER << " " << filename << std::endl;
    return true;
}
bool RequestSerializer::serialize_filesize_header(std::ostringstream& os, size_t filesize) {
    os << SIZE_HEADER << " " << filesize << std::endl;
    return true;
}
