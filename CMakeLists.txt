cmake_minimum_required(VERSION 3.22.1)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

project(adventure3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB GAME_DEF_FILES "game_def/*.hpp")
add_custom_command(
    OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/generated/game_def.hpp"
    COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/generate.sh"
    DEPENDS generate.sh
    DEPENDS ${GAME_DEF_FILES}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "generating game def files"
)
add_custom_target(gen_files
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/generated/game_def.hpp"
)
add_library(game_def INTERFACE)
target_include_directories(game_def
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/generated"
)
add_dependencies(game_def gen_files)

add_executable(adventure3
    src/cmd_parser.cpp
    src/commands.cpp
    src/dir_id.cpp
    src/display_state.cpp
    src/game_state.cpp
    src/get_user_input.cpp
    src/init_game.cpp
    src/main.cpp
    src/string_funcs.cpp
    src/tests.cpp
)
target_include_directories(adventure3 PRIVATE src game_def)
target_compile_options(adventure3 PRIVATE
    -Wall -Wextra -Werror -Wfatal-errors -pedantic-errors -Wpedantic
    -ggdb -O0 -g
)
add_definitions(
    "'-DROOM(...)='"
    "'-DDOOR(...)='"
    "'-DKEY(...)='"
    "'-DFURNITURE(...)='"
    "'-DPLACE(...)='"
    "'-DITEM(...)='"
    "'-DROOM_TRIGGER(...)='"
    "'-DUSE(...)='"
    "'-DROUTE(...)='"
    "'-DTELEPORT(...)='"
)
target_link_libraries(adventure3 PRIVATE game_def)
if (${RUN_TESTS})
    target_compile_definitions(adventure3 PRIVATE RUN_TESTS=1)
endif ()
