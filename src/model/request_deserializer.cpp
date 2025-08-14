#include "request_deserializer.hpp"

namespace p2ptransfer {

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

void RequestDeserializer::validate_method(std::string_view method_sv, RequestMethod required_method) {
    if(!request_methods_.contains(method_sv)) 
        throw std::runtime_error("no such method: " + SVMethods::to_string(method_sv));
    if(request_methods_.at(method_sv) != required_method) 
        throw std::runtime_error("Unexpected header found: " + SVMethods::to_string(method_sv));
}

}
