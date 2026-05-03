#include "cmd_parser.hpp"
#include "commands.hpp"
#include "display_state.hpp"
#include "game_state.hpp"
#include "get_user_input.hpp"
#include "init_game.hpp"
#include "tests.hpp"

int main() {
    run_tests();

    game_state gs = init_game();

    cmd_parser  p{get_cmd_table()};
    std::string input{"look"};

    for (;;) {
        if (!p.run_cmd(gs, input)) {
            gs.add_message("I don't understand.");
        }

        gs.fire_room_triggers();
        display_state(gs);
        gs.clear_messages();

        if (gs.is_complete()) {
            break;
        }

        input = get_user_input();
    }

    return 0;
}
