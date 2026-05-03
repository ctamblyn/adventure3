#pragma once

#include "entity.hpp"

#include <algorithm>
#include <map>

template <typename T> class property {
  private:
    std::map<entity, T>   m_values;
    static inline const T m_default{};

  public:
    void set(const entity &e, const T &s) { m_values[e] = s; }

    const T &get(const entity &e) const {
        const auto i = m_values.find(e);
        return (i == m_values.end()) ? m_default : i->second;
    }

    void erase(const entity &e) { m_values.erase(e); }

    void for_each(const auto &action) const {
        for (auto &v : m_values) {
            action(v);
        }
    }

    void for_each_with_rhs(T t, const auto &action) const {
        for (auto &v : m_values) {
            if (v.second == t) {
                action(v);
            }
        }
    }

    const std::pair<const entity, T> *find_first_if(const auto &pred) const {
        const auto iter = std::ranges::find_if(m_values, pred);
        return iter != m_values.end() ? &*iter : nullptr;
    }
};

#ifdef RUN_TESTS
#include "entity.hpp"
#include <cassert>
inline void test_property() {
    {
        const auto e = entity::new_entity();
        auto       p = property<char>{};
        assert(p.get(e) == char{});
    }
    {
        const auto e1 = entity::new_entity();
        const auto e2 = entity::new_entity();
        auto       p  = property<char>{};
        p.set(e1, 'a');
        assert(p.get(e1) == 'a');
        assert(p.get(e2) == char{});
    }
    {
        const auto e = entity::new_entity();
        auto       p = property<char>{};
        p.set(e, 'a');
        p.set(e, 'b');
        assert(p.get(e) == 'b');
    }
}
#endif
