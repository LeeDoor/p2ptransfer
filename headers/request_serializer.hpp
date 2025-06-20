#pragma once
#include "request_header_names.hpp"

class RequestSerializer {
public:
    static std::string serialize_send_request(const std::string& filename, size_t filesize);
    static std::string serialize_send_permission(const std::string filename);

private:
    static void serialize_send_request_header(std::ostringstream& os);
    static void serialize_permission_header(std::ostringstream& os);
    static void serialize_footer(std::ostringstream& os);
    static void serialize_filename_header(std::ostringstream& os, const std::string& filename);
    static void serialize_filesize_header(std::ostringstream& os, size_t filesize);
};
