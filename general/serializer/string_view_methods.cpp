#include "string_view_methods.hpp"

namespace general {
namespace serializer {

template<>
size_t SVMethods::deserialize_value(std::string_view line_sv) {
    auto line_end_ptr = line_sv.data() + line_sv.size();
    size_t conversion_result;
    auto result = std::from_chars(line_sv.data(), line_end_ptr, conversion_result);
    if(result.ec != std::errc() || result.ptr != line_end_ptr) 
        throw std::runtime_error("invalid unsigned long value: " + SVMethods::to_string(line_sv));
    return conversion_result;
}
template<>
std::string SVMethods::deserialize_value(std::string_view line_sv) {
    return SVMethods::to_string(line_sv);
}

template<>
void SVMethods::deserialize_value(std::string_view line_sv) {
    if(!line_sv.empty())
        throw std::runtime_error("expected empty data is not empty");
}

}
}
