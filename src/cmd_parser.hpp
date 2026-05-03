#pragma once

#include "game_state.hpp"

#include <string>

using cmd_handler = void (*)(game_state &,
                             const std::string &,
                             const std::string &);

struct cmd_tab_entry {
    const char *pattern{};
    const char *help_text{};
    unsigned    pos_1{};
    unsigned    pos_2{};
    cmd_handler handler{};
};

class cmd_parser {
  private:
    const cmd_tab_entry *begin{};
    const cmd_tab_entry *end{};

  public:
    template <typename ConstIter>
    cmd_parser(ConstIter cmd_tab_begin, ConstIter cmd_tab_end)
            : begin(&*cmd_tab_begin), end(&*cmd_tab_end) {}

    template <typename Container>
    cmd_parser(const Container &cmd_tab)
            : cmd_parser(std::begin(cmd_tab), std::end(cmd_tab)) {}

    bool run_cmd(game_state &gs, const std::string &str) const;
};
