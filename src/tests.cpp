#include "tests.hpp"

#include "entity.hpp"
#include "game_state.hpp"
#include "property.hpp"

#include <iostream>

#define TEST(n)                                                                \
    do {                                                                       \
        std::cout << "* " << #n << "\n" << std::flush;                         \
        test_##n();                                                            \
    } while (0)

void run_tests() {
#ifdef RUN_TESTS
    std::cout << "Running tests:\n" << std::flush;
    TEST(entity);
    TEST(property);
    TEST(game_state);
    std::cout << "All tests passed.\n" << std::flush;
#endif
}
