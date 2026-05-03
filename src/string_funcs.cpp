#include "string_funcs.hpp"

#include <algorithm>

std::string list_to_string(const std::vector<std::string> &list) {
    std::string str;
    for (std::size_t i = 0; i < list.size(); ++i) {
        str += list[i];
        if (i + 2 == list.size()) {
            str += " and ";
        } else if (i + 1 != list.size()) {
            str += ", ";
        }
    }
    return str;
}

bool is_plural(const std::string &str) {
    return !str.empty() && str.back() == 's';
}

bool starts_with_vowel(const std::string &str) {
    if (str.empty()) {
        return false;
    }
    const char ch = std::tolower(str.front());
    return ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u';
}

std::string add_prefix(const std::string &str) {
    const auto prefix = is_plural(str)         ? "some " :
                        starts_with_vowel(str) ? "an " :
                                                 "a ";
    return prefix + str;
}

std::string add_neg_prefix(const std::string &str) {
    const auto prefix = is_plural(str)         ? "any " :
                        starts_with_vowel(str) ? "an " :
                                                 "a ";
    return prefix + str;
}

std::string lowercase(const std::string &str) {
    std::string result{};
    std::transform(str.begin(),
                   str.end(),
                   std::back_inserter(result),
                   [](char ch) { return std::tolower(ch); });
    return result;
}

std::string trim_whitespace(const std::string &str) {
    const auto begin = std::find_if_not(
            str.begin(), str.end(), [](char ch) { return std::isspace(ch); });
    const auto end = std::find_if_not(str.rbegin(), str.rend(), [](char ch) {
                         return std::isspace(ch);
                     }).base();
    return std::string(begin, end);
}
