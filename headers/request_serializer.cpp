#include "request_serializer.hpp"
std::string RequestSerializer::serialize_send_request(const std::string& filename, 
                                   size_t filesize){
    std::ostringstream os;
    serialize_send_request_header(os);
    serialize_filename_header(os, filename);
    serialize_filesize_header(os, filesize);
    serialize_footer(os);
    return os.str();
}

std::string RequestSerializer::serialize_send_permission(const std::string filename){
    std::ostringstream os;
    serialize_permission_header(os);
    serialize_filename_header(os, filename);
    serialize_footer(os);
    return os.str();
}
void RequestSerializer::serialize_send_request_header(std::ostringstream& os) {
    os << REQUEST_HEADER << std::endl;
}
void RequestSerializer::serialize_permission_header(std::ostringstream& os) {
    os << PERMISSION_HEADER << std::endl;
}
void RequestSerializer::serialize_footer(std::ostringstream& os) {
    os << std::endl;
}
void RequestSerializer::serialize_filename_header(std::ostringstream& os, const std::string& filename) {
    os << FILE_HEADER << " " << filename << std::endl;
}

void RequestSerializer::serialize_filesize_header(std::ostringstream& os, size_t filesize) {
    os << SIZE_HEADER << " " << filesize << std::endl;
}
