#include "commands.hpp"

#include "dir_id.hpp"
#include "string_funcs.hpp"

#include <algorithm>

static void cmd_help(game_state &, const std::string &, const std::string &);
static void cmd_quit(game_state &, const std::string &, const std::string &);
static void cmd_inv(game_state &, const std::string &, const std::string &);
static void cmd_go(game_state &, const std::string &, const std::string &);
static void cmd_exam(game_state &, const std::string &, const std::string &);
static void cmd_look(game_state &, const std::string &, const std::string &);
static void cmd_get(game_state &, const std::string &, const std::string &);
static void cmd_drop(game_state &, const std::string &, const std::string &);
static void cmd_use(game_state &, const std::string &, const std::string &);

static const std::vector<cmd_tab_entry> cmd_tab{
        {.pattern   = "h|help",
         .help_text = "h, help              : show this help text",
         .handler   = cmd_help},

        {.pattern   = "q|quit",
         .help_text = "q, quit              : quit the game",
         .handler   = cmd_quit},

        {.pattern   = "i|inventory",
         .help_text = "i, inventory         : check your inventory",
         .handler   = cmd_inv},

        {.pattern   = "(l|look)",
         .help_text = "l, look              : look at your surroundings",
         .pos_1     = 0,
         .handler   = cmd_look},

        {.pattern   = "(go )?(n|north|e|east|s|south|w|west)",
         .help_text = "go north, n (etc.)   : move in direction",
         .pos_1     = 2,
         .handler   = cmd_go},

        {.pattern   = "(look at|examine) (.+)",
         .help_text = "examine <item>       : look at an item",
         .pos_1     = 2,
         .handler   = cmd_exam},

        {.pattern   = "(get|pick up) (.+)",
         .help_text = "get <item>           : pick up an item",
         .pos_1     = 2,
         .handler   = cmd_get},

        {.pattern   = "(drop|put down) (.+)",
         .help_text = "drop <item>          : put down an item",
         .pos_1     = 2,
         .handler   = cmd_drop},

        {.pattern   = "use (.+) on (.+)",
         .help_text = "use <item> on <item> : use an item on another item",
         .pos_1     = 1,
         .pos_2     = 2,
         .handler   = cmd_use},
};

////////////////////////////////////////////////////////////////////////////////

static void cmd_help(game_state &gs, const std::string &, const std::string &) {
    std::string msg;
    std::string delim;
    for (const auto &e : cmd_tab) {
        if (e.help_text && *e.help_text != '\0') {
            msg += delim + e.help_text;
            delim = "\n";
        }
    }
    gs.add_message(msg);
}

static void cmd_quit(game_state &gs, const std::string &, const std::string &) {
    gs.add_message("You bravely quit the game without completing it.");
    gs.set_complete();
}

static void cmd_inv(game_state &gs, const std::string &, const std::string &) {
    auto inventory_items = gs.get_inventory_item_names();
    if (inventory_items.empty()) {
        gs.add_message("You are not carrying anything.");
    } else {
        std::transform(inventory_items.begin(),
                       inventory_items.end(),
                       inventory_items.begin(),
                       add_prefix);
        gs.add_message("You are carrying " + list_to_string(inventory_items) +
                       ".");
    }
}

static void cmd_look(game_state &gs, const std::string &, const std::string &) {
    gs.add_message(gs.get_location_description());
    std::vector<std::string> visible_items = gs.get_visible_item_names();
    if (!visible_items.empty()) {
        std::transform(visible_items.begin(),
                       visible_items.end(),
                       visible_items.begin(),
                       add_prefix);
        gs.add_message("You can see " + list_to_string(visible_items) + ".");
    }
    const auto route_dir_names = gs.get_route_dir_names();
    if (!route_dir_names.empty()) {
        const std::string prefix = (route_dir_names.size() == 1) ?
                                           "There is a route to the " :
                                           "There are routes to the ";
        gs.add_message(prefix + list_to_string(route_dir_names) + ".");
    }
}

static void
cmd_go(game_state &gs, const std::string &arg_1, const std::string &) {
    const auto dir = string_to_dir_id(arg_1);
    if (!dir) {
        gs.add_message("That is not a way you can move.");
    } else {
        const auto dir_str = dir_id_to_string(*dir);
        if (gs.is_route_available_and_unlocked(*dir)) {
            gs.add_message("You go " + dir_str + ".");
            gs.move(*dir);
            cmd_look(gs, {}, {});
        } else if (gs.is_route_available(*dir)) {
            gs.add_message("The " + dir_str + " door is locked.");
        } else {
            gs.add_message("You cannot go " + dir_str + ".");
        }
    }
}

static void
cmd_exam(game_state &gs, const std::string &arg_1, const std::string &) {
    const auto e = gs.name_to_entity(arg_1);
    if (e && (gs.is_in_inventory(*e) || gs.is_in_location(*e))) {
        const auto desc = gs.get_description(*e);
        gs.add_message(desc);
    } else {
        gs.add_message("You cannot see " + add_neg_prefix(arg_1) + ".");
    }
}

static void
cmd_get(game_state &gs, const std::string &arg_1, const std::string &) {
    const auto e = gs.name_to_entity(arg_1);
    if (!e) {
        gs.add_message("You cannot see " + add_neg_prefix(arg_1) + ".");
    } else if (gs.is_in_inventory(*e)) {
        gs.add_message("You already have the " + arg_1 + ".");
    } else if (!gs.is_in_location(*e)) {
        gs.add_message("You cannot see " + add_neg_prefix(arg_1) + ".");
    } else if (!gs.is_carriable(*e)) {
        gs.add_message("You cannot pick up the " + arg_1 + ".");
    } else {
        gs.add_message("You pick up the " + arg_1 + ".");
        gs.move_to_inventory(*e);
    }
}

static void
cmd_drop(game_state &gs, const std::string &arg_1, const std::string &) {
    const auto e = gs.name_to_entity(arg_1);
    if (!e || !gs.is_in_inventory(*e)) {
        gs.add_message("You do not have " + add_neg_prefix(arg_1) + ".");
    } else {
        gs.add_message("You drop the " + arg_1 + ".");
        gs.move_to_location(*e);
    }
}

static void
cmd_use(game_state &gs, const std::string &arg_1, const std::string &arg_2) {
    const auto e1 = gs.name_to_entity(arg_1);
    const auto e2 = gs.name_to_entity(arg_2);
    if (!e1 || !gs.is_in_inventory(*e1)) {
        gs.add_message("You do not have " + add_neg_prefix(arg_1) + ".");
    } else if (!e2 || (!gs.is_in_inventory(*e2) && !gs.is_in_location(*e2))) {
        gs.add_message("You cannot see " + add_neg_prefix(arg_2) + ".");
    } else {
        const auto &func = gs.get_use_on(*e1);
        if (!func || !func(gs, *e2)) {
            gs.add_message("You cannot use the " + arg_1 + " on the " + arg_2 +
                           ".");
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

const std::vector<cmd_tab_entry> &get_cmd_table() { return cmd_tab; }
