#include "request_deserializer.hpp"
const std::unordered_map<HeaderType, RequestMethod> RequestDeserializer::request_methods_ = {
        { REQUEST_HEADER, RequestMethod::REQUEST },
        { FILE_HEADER, RequestMethod::FILENAME },
        { SIZE_HEADER, RequestMethod::FILESIZE },
        { PERMISSION_HEADER, RequestMethod::PERMISSION },
    };

SendRequest RequestDeserializer::deserialize_send_request(const std::string request_str){
    SendRequest send_request;
    std::string_view request_sv(request_str);
    deserialize_line<void>(SVMethods::pop_line(request_sv), RequestMethod::REQUEST);
    send_request.filename = deserialize_line<Filename>(SVMethods::pop_line(request_sv), RequestMethod::FILENAME);
    send_request.filesize = deserialize_line<Filesize>(SVMethods::pop_line(request_sv), RequestMethod::FILESIZE);
    return send_request;
}

RequestMethod RequestDeserializer::deserialize_method(std::string_view method_sv) {
    if(!request_methods_.contains(method_sv)) 
        throw std::runtime_error("no such method: " + SVMethods::to_string(method_sv));
    return request_methods_.at(method_sv);
}
template<>
size_t RequestDeserializer::deserialize_value(std::string_view line_sv) {
    auto line_end_ptr = line_sv.data() + line_sv.size();
    size_t conversion_result;
    auto result = std::from_chars(line_sv.data(), line_end_ptr, conversion_result);
    if(result.ec != std::errc() || result.ptr != line_end_ptr) 
        throw std::runtime_error("invalid unsigned long value: " + SVMethods::to_string(line_sv));
    return conversion_result;
}
template<>
std::string RequestDeserializer::deserialize_value(std::string_view line_sv) {
    return SVMethods::to_string(line_sv);
}

template<>
void RequestDeserializer::deserialize_value(std::string_view line_sv) {
    if(!line_sv.empty())
        throw std::runtime_error("expected empty data is not empty");
}
