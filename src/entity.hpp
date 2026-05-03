#pragma once

#include <atomic>

class entity {
  private:
    static inline std::atomic<unsigned> max_id{1};
    unsigned                            m_id{0};
    constexpr entity(unsigned id) : m_id{id} {}

  public:
    constexpr entity() = default;

    static constexpr entity default_entity() { return entity{}; }
    static entity           new_entity() { return entity{max_id.fetch_add(1)}; }

    constexpr unsigned get_id() const { return m_id; }

    constexpr bool operator<(const entity &e) const { return m_id < e.m_id; }

    constexpr bool operator==(const entity &e) const { return m_id == e.m_id; }
};

#ifdef RUN_TESTS
#include <cassert>
inline void test_entity() {
    {
        entity e1{entity::default_entity()};
        entity e2{entity::default_entity()};
        assert(e1.get_id() == 0);
        assert(e2.get_id() == 0);
    }

    {
        entity e1{entity::new_entity()};
        entity e2{entity::default_entity()};
        assert(e1.get_id() != e2.get_id());
    }

    {
        entity e1{entity::default_entity()};
        entity e2{entity::new_entity()};
        assert(e1.get_id() != e2.get_id());
    }

    {
        entity e1{entity::new_entity()};
        entity e2{entity::new_entity()};
        assert(e1.get_id() != e2.get_id());
    }
}
#endif
