#pragma once
#include <optional>
#include <sstream>

class RequestSerializer {
public:
    static std::optional<std::string> serialize_send_request(const std::string& filename, 
                                                      size_t filesize);
    static std::optional<std::string> serialize_send_permission(const std::string filename);
private:
    static bool serialize_filename_header(std::ostringstream& os, const std::string& filename);
    static bool serialize_filesize_header(std::ostringstream& os, size_t filesize);
};
