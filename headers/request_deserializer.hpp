#pragma once
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
    static SendRequest deserialize_send_request(const std::string request_str);
private:
    RequestDeserializer() = delete;
    template<DeserializableOrVoid ValueType> // if you don't need a value, use std::monostate
    static ValueType deserialize_line(std::istringstream& is, RequestMethod required_method){
        RequestMethod method = deserialize_method(is);
        if(method != required_method) 
            throw std::runtime_error("failed while deserializing the method of header " + std::string(typeid(ValueType).name()));
        if constexpr(std::is_same_v<std::monostate, ValueType>) {
            return ValueType{};
        } else {
            ValueType value;
            is >> value;
            if(!is.good())
                throw std::runtime_error("failed while deserializing the value of header " + std::string(typeid(ValueType).name()));
            return value;
        }
    }
    static RequestMethod deserialize_method(std::istringstream& is);

    static const std::unordered_map<HeaderType, RequestMethod> request_methods_;
};
