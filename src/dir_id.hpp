#pragma once

#include <optional>
#include <string>

enum class dir_id { north, east, south, west };

std::optional<dir_id> string_to_dir_id(const std::string &str);
std::string           dir_id_to_string(dir_id dir);
dir_id                reverse(dir_id dir);
