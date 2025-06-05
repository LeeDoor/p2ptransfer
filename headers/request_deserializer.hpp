#pragma once
#include "logger.hpp"
#include "request_header_names.hpp"
#include "request_structures.hpp"
template<typename T>
concept Deserializable = requires(std::istringstream& is, T& v){
is >> v;
};
template<typename T>
concept DeserializableOrVoid = Deserializable<T> || std::is_same_v<T, std::monostate>;
class RequestDeserializer {
public:
    RequestDeserializer(){
        request_methods_ = {
            { REQUEST_HEADER, RequestMethod::REQUEST },
            { FILE_HEADER, RequestMethod::FILENAME },
            { SIZE_HEADER, RequestMethod::FILESIZE },
            { PERMISSION_HEADER, RequestMethod::PERMISSION },
        };
    }

    std::optional<SendRequest> deserialize_send_request(const std::string request_str){
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
        if(auto filesize = deserialize_value<Filesize>(is, RequestMethod::FILESIZE)) {
            sr.filesize = *filesize;
        } else {
            Logger::log() << "failed while deserializing the filesize." << std::endl;
            return std::nullopt;
        }
        return sr;
    }
private:
    template<DeserializableOrVoid ValueType> // if you don't need a value, use std::monostate
    std::optional<ValueType> deserialize_value(std::istringstream& is, RequestMethod required_method){
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
    std::optional<RequestMethod> deserialize_method(std::istringstream& is){
        std::string method_line;
        is >> method_line;
        if(!request_methods_.contains(method_line)) {
            return std::nullopt;
        }
        return request_methods_.at(method_line);
    }

    std::unordered_map<HeaderType, RequestMethod> request_methods_;
};
