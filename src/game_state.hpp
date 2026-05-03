#pragma once

#include "dir_id.hpp"
#include "entity.hpp"
#include "property.hpp"

#include <functional>
#include <optional>
#include <string>
#include <vector>

class game_state {
  public:
    using use_on       = std::function<bool(game_state &, entity)>;
    using room_trigger = std::function<void(game_state &)>;

    void                            add_message(const std::string &msg);
    const std::vector<std::string> &get_messages() const;
    void                            clear_messages();

    bool is_complete() const;
    void set_complete();

    std::vector<std::string> get_inventory_item_names() const;
    std::vector<std::string> get_visible_item_names() const;
    std::vector<std::string> get_route_dir_names() const;
    const std::string       &get_location_description() const;
    bool                     is_route_available(dir_id dir) const;
    bool                     is_route_available_and_unlocked(dir_id dir) const;
    std::optional<entity>    name_to_entity(const std::string &name) const;
    bool                     is_in_inventory(entity e) const;
    bool                     is_in_location(entity e) const;
    const std::string       &get_description(entity e) const;
    bool                     is_carriable(entity e) const;
    const use_on            &get_use_on(entity e) const;

    void move(dir_id dir);
    void teleport(entity e);
    void move_to_location(entity e);
    void move_to_location(entity e, entity location);
    void move_to_inventory(entity e);
    void hide(entity e);
    void fire_room_triggers();
    void set_use_on(entity e, const use_on &use_on_func);
    void set_room_trigger(entity e, const room_trigger &trigger_func);
    void set_description(entity e, const std::string &description);

    entity create_room(const std::string  &description,
                       const room_trigger &trigger_func = nullptr);
    std::pair<entity, entity>
           create_locked_door(entity r_from, dir_id dir, entity r_to);
    entity create_route(entity r_from, dir_id dir, entity r_to);
    entity create_item(const std::string &name,
                       const std::string &description,
                       const use_on      &use_on_func = nullptr);
    entity create_furniture(const std::string &name,
                            const std::string &description,
                            const use_on      &use_on_func = nullptr);
    entity create_key(const std::string &name,
                      const std::string &description,
                      entity             door_face);

  private:
    // items, door faces, inventory
    property<std::string> m_name;

    // items, door faces, rooms
    property<std::string> m_description;

    // items, door faces
    property<bool>   m_is_carriable;
    property<entity> m_location;

    // type tags
    property<bool> m_is_item;
    property<bool> m_is_room;
    property<bool> m_is_door_face;

    // route parents
    property<bool> m_is_locked;
    property<bool> m_is_blocked;

    // routes, door faces
    // (routes are always unlocked and have no description)
    property<dir_id> m_direction;
    property<entity> m_route_parent;

    // rooms
    property<room_trigger> m_room_trigger;

    // use <instrument> on <patient>
    property<use_on> m_use_on;

    // player state
    entity m_player_inventory{entity::new_entity()};
    entity m_player_location{entity::new_entity()};
    bool   m_is_complete{false};

    // messages
    std::vector<std::string> m_messages;
};

#ifdef RUN_TESTS
#include <cassert>
inline void test_game_state() {
    // TODO
}
#endif
