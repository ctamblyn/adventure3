#pragma once

#include <string>
#include <vector>

std::string list_to_string(const std::vector<std::string> &list);
bool        is_plural(const std::string &str);
bool        starts_with_vowel(const std::string &str);
std::string add_prefix(const std::string &str);
std::string add_neg_prefix(const std::string &str);
std::string lowercase(const std::string &str);
std::string trim_whitespace(const std::string &str);
