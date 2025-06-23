#pragma once
#include "request_header_names.hpp"
#include "request_structures.hpp"
#include "string_view_methods.hpp"
template<typename T>
concept Deserializable = requires(std::istringstream& is, T& v){
is >> v;
};
template<typename T>
concept DeserializableOrVoid = Deserializable<T> || std::is_same_v<T, void>;
class RequestDeserializer {
public:
    static SendRequest deserialize_send_request(const std::string request_str);
private:
    RequestDeserializer() = delete;
    template<DeserializableOrVoid ValueType>
    static ValueType deserialize_line(std::string_view line, RequestMethod required_method){
        RequestMethod method = deserialize_method(SVMethods::pop_word(line));
        if(method != required_method) 
            throw std::runtime_error("Unexpected header found");
        std::string_view value_sv = SVMethods::pop_line(line);
        if constexpr (std::is_same_v<ValueType, void>) {
            return deserialize_value<ValueType>(value_sv);
        } else {
            ValueType value = deserialize_value<ValueType>(value_sv);
            return value;
        }
    }
    static RequestMethod deserialize_method(std::string_view method_sv);
    template<DeserializableOrVoid ValueType>
    static ValueType deserialize_value(std::string_view line_sv);

    static const std::unordered_map<HeaderType, RequestMethod> request_methods_;
};
