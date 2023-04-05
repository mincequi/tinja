#pragma once

#include <string>

typedef struct moustache_variable {
    const char *key;
    std::string value;
} moustache_variable_t;

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

template <size_t n>
inline std::string moustache_render(const std::string &format, moustache_variable_t (&values)[n]) {
    auto result = format;
    // Conditional sections
    for (size_t i = 0; i < n; i++) {
        // Include Section {{#expr}}
        const auto match_section_begin = "{{#" + std::string(values[i].key) + "}}";
        // Inverted section {{^expr}}
        const auto match_section_inverted_begin = "{{^" + std::string(values[i].key) + "}}";
        // End section {{/expr}}
        const auto match_section_end = "{{/" + std::string(values[i].key) + "}}";
        while (true) {
            bool inverted = false;
            auto first = result.find(match_section_begin);
            if (first == std::string::npos)
            {
                inverted = true;
                first = result.find(match_section_inverted_begin);
                if (first == std::string::npos)
                    break;
            }

            auto second = result.find(match_section_end, first + match_section_begin.length());
            if (second == std::string::npos)
                break;

            // Arduino returns 0 and 1 for bool.toString()
            if ((!inverted && (values[i].value == "1")) || (inverted && (values[i].value == "0")))
                result = result.substr(0, first) + result.substr(first + match_section_begin.length(), second) + result.substr(second + match_section_end.length());
            else
                result = result.substr(0, first) + result.substr(second + match_section_end.length());
        }
    }

    // Replace variables {{variable}}
    for (size_t i = 0; i < n; i++) {
        while (replace(result, "{{" + std::string(values[i].key) + "}}", values[i].value)) {
        }
    }

    return result;
}
