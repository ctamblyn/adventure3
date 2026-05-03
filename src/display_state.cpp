#include "display_state.hpp"

#include <iostream>
#include <thread>

void display_state(const game_state &gs) {
    using namespace std::chrono_literals;

    bool delim = false;

    for (const auto &msg : gs.get_messages()) {
        if (!delim) {
            delim = true;
        } else {
            std::cout << '\n';
        }

        for (const auto &c : msg) {
            std::cout << c << std::flush;
            std::this_thread::sleep_for(10ms);
        }

        std::cout << '\n';
    }
}
