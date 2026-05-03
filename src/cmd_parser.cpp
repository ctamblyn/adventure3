#include "cmd_parser.hpp"

#include <algorithm>
#include <regex>

bool cmd_parser::run_cmd(game_state &gs, const std::string &str) const {
    std::smatch m{};

    const auto iter = std::find_if(begin, end, [&str, &m](const auto &e) {
        return std::regex_match(str, m, std::regex(e.pattern));
    });

    if (iter != end) {
        std::string arg_1{};
        std::string arg_2{};

        if (m.size() > iter->pos_1) {
            arg_1 = m[iter->pos_1].str();
            if (m.size() > iter->pos_2) {
                arg_2 = m[iter->pos_2].str();
            }
        }

        iter->handler(gs, arg_1, arg_2);
        return true;
    } else {
        return false;
    }
}
