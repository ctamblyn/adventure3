#include "get_user_input.hpp"

#include "string_funcs.hpp"

#include <iostream>

std::string get_user_input() {
    std::cout << "> ";
    std::cout.flush();

    std::string cmd;
    if (!std::getline(std::cin, cmd)) {
        return {};
    }

    return lowercase(trim_whitespace(cmd));
}
