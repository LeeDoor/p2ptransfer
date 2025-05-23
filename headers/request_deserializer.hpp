#pragma once
#include "request_header_names.hpp"
#include "request_structures.hpp"
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
template<typename T>
concept Deserializable = requires(std::istringstream& is, T& v){
    is >> v;
};
template<typename T>
concept DeserializableOrVoid = Deserializable<T> || std::is_same_v<T, std::monostate>;
class RequestDeserializer {
public:
    RequestDeserializer();
    std::optional<SendRequest> deserialize_send_request(const std::string request_str);
private:
    template<DeserializableOrVoid ValueType> // if you don't need a value, use std::monostate
    std::optional<ValueType> deserialize_value(std::istringstream& is, RequestMethod required_method);
    std::optional<RequestMethod> deserialize_method(std::istringstream& is);

    std::unordered_map<HeaderType, RequestMethod> request_methods_;
};
