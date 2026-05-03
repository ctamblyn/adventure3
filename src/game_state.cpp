#include "game_state.hpp"

#include "string_funcs.hpp"

void game_state::add_message(const std::string &msg) {
    m_messages.push_back(trim_whitespace(msg));
}

const std::vector<std::string> &game_state::get_messages() const {
    return m_messages;
}

void game_state::clear_messages() { m_messages.clear(); }

bool game_state::is_complete() const { return m_is_complete; }

void game_state::set_complete() { m_is_complete = true; }

std::vector<std::string> game_state::get_inventory_item_names() const {
    std::vector<std::string> names;
    m_location.for_each_with_rhs(m_player_inventory, [&](auto &x) {
        names.push_back(m_name.get(x.first));
    });
    return names;
}

std::vector<std::string> game_state::get_visible_item_names() const {
    std::vector<std::string> names;
    m_location.for_each_with_rhs(m_player_location, [&](auto &x) {
        if (m_is_carriable.get(x.first)) {
            names.push_back(m_name.get(x.first));
        }
    });
    return names;
}

std::vector<std::string> game_state::get_route_dir_names() const {
    std::vector<std::string> names;
    m_location.for_each_with_rhs(m_player_location, [&](auto &x) {
        if (m_is_door_face.get(x.first)) {
            names.push_back(dir_id_to_string(m_direction.get(x.first)));
        }
    });
    return names;
}

const std::string &game_state::get_location_description() const {
    return m_description.get(m_player_location);
}

bool game_state::is_route_available(dir_id dir) const {
    return m_location.find_first_if([&](const auto &x) {
        return m_is_door_face.get(x.first) && x.second == m_player_location &&
               m_direction.get(x.first) == dir;
    });
}

bool game_state::is_route_available_and_unlocked(dir_id dir) const {
    return m_location.find_first_if([&](const auto x) {
        return m_is_door_face.get(x.first) && x.second == m_player_location &&
               m_direction.get(x.first) == dir &&
               !m_is_locked.get(m_route_parent.get(x.first));
    });
}

std::optional<entity>
game_state::name_to_entity(const std::string &name) const {
    const auto e = m_name.find_first_if([&](auto &x) {
        const auto loc = m_location.get(x.first);
        return (loc == m_player_location || loc == m_player_inventory) &&
               x.second == name;
    });
    if (e) {
        return e->first;
    } else {
        return std::nullopt;
    }
}

bool game_state::is_in_inventory(entity e) const {
    return m_location.get(e) == m_player_inventory;
}

bool game_state::is_in_location(entity e) const {
    return m_location.get(e) == m_player_location;
}

const std::string &game_state::get_description(entity e) const {
    return m_description.get(e);
}

bool game_state::is_carriable(entity e) const { return m_is_carriable.get(e); }

const game_state::use_on &game_state::get_use_on(entity e) const {
    return m_use_on.get(e);
}

void game_state::move(dir_id dir) {
    const auto from_face = m_location.find_first_if([&](auto &x) {
        return x.second == m_player_location && m_is_door_face.get(x.first) &&
               m_direction.get(x.first) == dir;
    });
    const auto parent    = m_route_parent.get(from_face->first);
    const auto to_face   = m_route_parent.find_first_if([&](auto &x) {
        return x.second == parent && x.first != from_face->first;
    });
    m_player_location    = m_location.get(to_face->first);
}

void game_state::teleport(entity e) { m_player_location = e; }

void game_state::move_to_location(entity e) {
    m_location.set(e, m_player_location);
}

void game_state::move_to_location(entity e, entity location) {
    m_location.set(e, location);
}

void game_state::move_to_inventory(entity e) {
    m_location.set(e, m_player_inventory);
}

void game_state::hide(entity e) { m_location.erase(e); }

void game_state::fire_room_triggers() {
    const auto func = m_room_trigger.get(m_player_location);
    if (func) {
        func(*this);
    }
}

void game_state::set_use_on(entity e, const use_on &use_on_func) {
    m_use_on.set(e, use_on_func);
}

void game_state::set_room_trigger(entity e, const room_trigger &trigger_func) {
    m_room_trigger.set(e, trigger_func);
}

void game_state::set_description(entity e, const std::string &description) {
    m_description.set(e, description);
}

entity game_state::create_room(const std::string  &description,
                               const room_trigger &trigger_func) {
    const auto e = entity::new_entity();
    m_description.set(e, description);
    m_is_room.set(e, true);
    if (trigger_func) {
        m_room_trigger.set(e, trigger_func);
    }
    return e;
}

std::pair<entity, entity>
game_state::create_locked_door(entity r_from, dir_id dir, entity r_to) {
    const auto from = entity::new_entity();
    m_name.set(from, dir_id_to_string(dir) + " door");
    m_description.set(from, "A normal door.");
    m_direction.set(from, dir);
    m_location.set(from, r_from);
    m_is_door_face.set(from, true);
    const auto to = entity::new_entity();
    m_name.set(to, dir_id_to_string(reverse(dir)) + " door");
    m_description.set(to, "A normal door.");
    m_direction.set(to, reverse(dir));
    m_location.set(to, r_to);
    m_is_door_face.set(to, true);
    const auto parent = entity::new_entity();
    m_route_parent.set(from, parent);
    m_route_parent.set(to, parent);
    m_is_locked.set(parent, true);
    return {from, to};
}

entity game_state::create_route(entity r_from, dir_id dir, entity r_to) {
    const auto from = entity::new_entity();
    m_direction.set(from, dir);
    m_location.set(from, r_from);
    m_is_door_face.set(from, true);
    const auto to = entity::new_entity();
    m_direction.set(to, reverse(dir));
    m_location.set(to, r_to);
    m_is_door_face.set(to, true);
    const auto parent = entity::new_entity();
    m_route_parent.set(from, parent);
    m_route_parent.set(to, parent);
    return parent;
}

entity game_state::create_item(const std::string &name,
                               const std::string &description,
                               const use_on      &use_on_func) {
    const auto e = entity::new_entity();
    m_name.set(e, name);
    m_description.set(e, description);
    m_is_carriable.set(e, true);
    m_is_item.set(e, true);
    if (use_on_func) {
        m_use_on.set(e, use_on_func);
    }
    return e;
}

entity game_state::create_furniture(const std::string &name,
                                    const std::string &description,
                                    const use_on      &use_on_func) {
    const auto e = entity::new_entity();
    m_name.set(e, name);
    m_description.set(e, description);
    m_is_carriable.set(e, false);
    m_is_item.set(e, true);
    if (use_on_func) {
        m_use_on.set(e, use_on_func);
    }
    return e;
}

entity game_state::create_key(const std::string &name,
                              const std::string &description,
                              entity             door_face) {
    const auto door_parent = m_route_parent.get(door_face);
    const auto use_key = [door_parent, this](game_state &, entity door_face) {
        if (!m_is_door_face.get(door_face)) {
            return false;
        }
        const auto parent = m_route_parent.get(door_face);
        if (parent != door_parent) {
            add_message("The key doesn't fit.");
        } else if (m_is_locked.get(parent)) {
            if (m_is_blocked.get(parent)) {
                add_message("The lock seems to be blocked by something.");
            } else {
                add_message("You unlock the door.");
                m_is_locked.set(door_parent, false);
            }
        } else {
            add_message("You lock the door.");
            m_is_locked.set(door_parent, true);
        }
        return true;
    };
    return create_item(name, description, use_key);
}
