///////////////////////////////////////////////////////////////////////////////

ROOM(tool_room, R"(
You are in a small store room used for storing tools.
)")

///////////////////////////////////////////////////////////////////////////////

ITEM(spanner, "spanner", R"(
A rusty spanner.
)")

PLACE(spanner, tool_room)

///////////////////////////////////////////////////////////////////////////////

ITEM(screwdriver, "screwdriver", R"(
A flat-headed screwdriver with a blue handle.
)")

PLACE(screwdriver, tool_room)

USE(screwdriver, {
    // TODO
    (void)gs;
    (void)patient;
    return false;
})

///////////////////////////////////////////////////////////////////////////////

ITEM(hammer, "hammer", R"(
An old but robust hammer.  Mind your thumb!
)")

PLACE(hammer, tool_room)

USE(hammer, {
    if (patient != vase) {
        return false;
    }

    gs.add_message(R"(
You smash the expensive vase with the hammer.

Among the fragments you see a silver key!
    )");

    gs.hide(vase);
    gs.move_to_location(fragments);
    gs.move_to_location(silver_key);
    return true;
})

///////////////////////////////////////////////////////////////////////////////
