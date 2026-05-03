#!/bin/bash
cd game_def || exit 1

cat <<EOF > ../generated/game_def.hpp
// generated code - do not edit
#ifndef ROOM
#define ROOM(...)
#endif
#ifndef DOOR
#define DOOR(...)
#endif
#ifndef KEY
#define KEY(...)
#endif
#ifndef FURNITURE
#define FURNITURE(...)
#endif
#ifndef PLACE
#define PLACE(...)
#endif
#ifndef ITEM
#define ITEM(...)
#endif
#ifndef ROOM_TRIGGER
#define ROOM_TRIGGER(...)
#endif
#ifndef USE
#define USE(...)
#endif
#ifndef ROUTE
#define ROUTE(...)
#endif
#ifndef TELEPORT
#define TELEPORT(...)
#endif
EOF

find . -name "*.hpp" -exec echo '#include "'{}'"' \; >> ../generated/game_def.hpp
