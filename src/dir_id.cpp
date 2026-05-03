#include "dir_id.hpp"

std::optional<dir_id> string_to_dir_id(const std::string &str) {
    return (str == "n" || str == "north") ? std::optional(dir_id::north) :
           (str == "e" || str == "east")  ? std::optional(dir_id::east) :
           (str == "s" || str == "south") ? std::optional(dir_id::south) :
           (str == "w" || str == "west")  ? std::optional(dir_id::west) :
                                            std::nullopt;
}

std::string dir_id_to_string(dir_id dir) {
    return (dir == dir_id::north) ? "north" :
           (dir == dir_id::east)  ? "east" :
           (dir == dir_id::south) ? "south" :
                                    "west";
}

dir_id reverse(dir_id dir) {
    return (dir == dir_id::north) ? dir_id::south :
           (dir == dir_id::east)  ? dir_id::west :
           (dir == dir_id::south) ? dir_id::north :
                                    dir_id::east;
}
