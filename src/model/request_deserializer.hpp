#pragma once
#include "request_header_names.hpp"
#include "request_structures.hpp"
#include "string_view_methods.hpp"

namespace p2ptransfer {

class RequestDeserializer {
public:
    static SendRequest deserialize_send_request(const std::string request_str);
private:
    RequestDeserializer() = delete;
    template<DeserializableOrVoid ValueType>
    static ValueType deserialize_line(std::string_view line, RequestMethod required_method){
        validate_method(SVMethods::pop_word(line), required_method);
        std::string_view value_sv = SVMethods::pop_line(line);
        if constexpr (std::is_same_v<ValueType, void>) {
            return SVMethods::deserialize_value<ValueType>(value_sv);
        } else {
            ValueType value = SVMethods::deserialize_value<ValueType>(value_sv);
            return value;
        }
    }
    static void validate_method(std::string_view method_sv, RequestMethod required_method);

    static const std::unordered_map<HeaderType, RequestMethod> request_methods_;
};

}
