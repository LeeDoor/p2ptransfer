#include "request_deserializer.hpp"
RequestDeserializer::RequestDeserializer(){
    request_methods_ = {
        { REQUEST_HEADER, RequestMethod::REQUEST },
        { FILE_HEADER, RequestMethod::FILENAME },
        { SIZE_HEADER, RequestMethod::FILESIZE },
        { PERMISSION_HEADER, RequestMethod::PERMISSION },
    };
}

SendRequest RequestDeserializer::deserialize_send_request(const std::string request_str){
    std::istringstream is(request_str);
    SendRequest send_request;
    deserialize_line<std::monostate>(is, RequestMethod::REQUEST);
    send_request.filename = deserialize_line<Filename>(is, RequestMethod::FILENAME);
    send_request.filesize = deserialize_line<Filesize>(is, RequestMethod::FILESIZE);
    return send_request;
}
RequestMethod RequestDeserializer::deserialize_method(std::istringstream& is){
    std::string method_line;
    is >> method_line;
    if(!request_methods_.contains(method_line))
        throw std::runtime_error("deserialized header " + method_line + " cant be matched with any header");
    return request_methods_.at(method_line);
}

