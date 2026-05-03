#include "init_game.hpp"

#include "entity.hpp"
#include "game_state.hpp"
#include "string_funcs.hpp"

////////////////////////////////////////////////////////////////////////////////

game_state init_game() {
    game_state gs;

// rooms
#include "undefine_macros.hpp"
#define ROOM(ident, desc)                                                      \
    const auto ident{gs.create_room(trim_whitespace(desc))};
#include "game_def.hpp"

// routes and doors
#include "undefine_macros.hpp"
#define ROUTE(from, direction, to) gs.create_route(from, dir_id::direction, to);
#define DOOR(ident, from, direction, to)                                       \
    const auto ident{gs.create_locked_door(from, dir_id::direction, to).first};
#include "game_def.hpp"

// items
#include "undefine_macros.hpp"
#define KEY(ident, target_door, name, description)                             \
    const auto ident{gs.create_key(name, description, target_door)};
#define FURNITURE(ident, name, description)                                    \
    const auto ident{gs.create_furniture(name, description)};
#define ITEM(ident, name, description)                                         \
    const auto ident{gs.create_item(name, description)};
#define PLACE(entity, location) gs.move_to_location(entity, location);
#include "game_def.hpp"

// callbacks
#include "undefine_macros.hpp"
#define ROOM_TRIGGER(room, ...)                                                \
    gs.set_room_trigger(room, [](game_state & gs) __VA_ARGS__);
#define USE(instrument, ...)                                                   \
    gs.set_use_on(instrument, [=](game_state & gs, entity patient) __VA_ARGS__);
#include "game_def.hpp"

// initial player location
#include "undefine_macros.hpp"
#define TELEPORT(location) gs.teleport(location);
#include "game_def.hpp"

    return gs;
}
