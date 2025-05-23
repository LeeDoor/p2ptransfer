#include <cstring>
#include <string>
#include <sstream>
#include <variant>
#include "request_deserializer.hpp"
#include "request_header_names.hpp"
#include "logger.hpp"
RequestDeserializer::RequestDeserializer() {
    request_methods_ = {
        { REQUEST_HEADER, RequestMethod::REQUEST },
        { FILE_HEADER, RequestMethod::REQUEST },
        { SIZE_HEADER, RequestMethod::SIZE },
        { PERMISSION_HEADER, RequestMethod::PERMISSION },
    };
}
std::optional<SendRequest> RequestDeserializer::deserialize_send_request(const std::string request_str) {
    Logger::log() << "handling request: " << request_str << std::endl;
    std::istringstream is(request_str);
    SendRequest sr;
    if(!deserialize_value<std::monostate>(is, RequestMethod::REQUEST)) {
        Logger::log() << "failed while deserializing the main method." << std::endl;
        return std::nullopt;
    }
    if(auto filename = deserialize_value<Filename>(is, RequestMethod::FILENAME)) {
        sr.filename = *filename;
    } else {
        Logger::log() << "failed while deserializing the filename." << std::endl;
        return std::nullopt;
    }
    if(auto filesize = deserialize_value<Filesize>(is, RequestMethod::SIZE)) {
        sr.filesize = *filesize;
    } else {
        Logger::log() << "failed while deserializing the filesize." << std::endl;
        return std::nullopt;
    }
    return sr;
}
template<DeserializableOrVoid ValueType>
std::optional<ValueType> RequestDeserializer::deserialize_value(std::istringstream& is, RequestMethod required_method) {
    if(std::optional<RequestMethod> method_opt = deserialize_method(is);
        !method_opt ||*method_opt != required_method) {
        Logger::log() << "failed while deserializing method of header " << typeid(ValueType).name() << std::endl;
        return std::nullopt;
    }
    if constexpr(std::is_same_v<std::monostate, ValueType>) {
        return ValueType{};
    } else {
        ValueType value;
        is >> value;
        if(!is.good()) {
            Logger::log() << "failed while deserializing the value of header " << typeid(ValueType).name() << std::endl;
            return std::nullopt;
        }
        return value;
    }
}
std::optional<RequestMethod> RequestDeserializer::deserialize_method(std::istringstream& is) {
    std::string method_line;
    is >> method_line;
    Logger::log() << "method: " << method_line << std::endl;
    if(!request_methods_.contains(method_line)) {
        return std::nullopt;
    }
    return request_methods_.at(method_line);
}
