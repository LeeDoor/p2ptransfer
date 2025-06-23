#pragma once

template<typename T>
concept Deserializable = requires(std::istringstream& is, T& v){
is >> v;
};
template<typename T>
concept DeserializableOrVoid = Deserializable<T> || std::is_same_v<T, void>;
class SVMethods {
public:
    static std::string_view pop_substr_skip_delim(std::string_view& sv, std::string_view delim) {
        auto pos = sv.find(delim);
        if(pos == std::string_view::npos) 
            throw std::runtime_error("unable to find " + to_string(delim) + 
                                     " substr in \"" + to_string(sv) + "\"");
        auto res = sv.substr(0, pos);
        sv.remove_prefix(pos + delim.size());
        return res;
    }
    static std::string_view pop_line(std::string_view& sv) {
        try {
            return pop_substr_skip_delim(sv, "\n");
        } catch (const std::runtime_error&) {
            return move_itself(sv);
        }
    }
    static std::string_view pop_word(std::string_view& sv) {
        try {
            return pop_substr_skip_delim(sv, " ");   
        } catch (const std::runtime_error&) {
            return move_itself(sv);
        }
    }
    static std::string to_string(std::string_view sv) {
        return std::string(sv.data(), sv.size());
    }
    template<DeserializableOrVoid ValueType>
    static ValueType deserialize_value(std::string_view line_sv);

private:
    static std::string_view move_itself(std::string_view& sv) {
        std::string_view sv_copy = sv;
        sv.remove_prefix(sv.size());
        return sv_copy;
    }
};
