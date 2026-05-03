///////////////////////////////////////////////////////////////////////////////

ROOM(living_room, R"(
You are in a living room.

By the far wall, you see a sofa.  There is a small fireplace, but no fire.  On
the wall is a picture of a rural landscape.  There is a set of shelves on which
have been placed several small ornaments.
)")

///////////////////////////////////////////////////////////////////////////////

FURNITURE(fireplace, "fireplace", R"(
The fireplace is cold.  There is a little ash in the bottom, and cobwebs over
the chimney opening.
)")

PLACE(fireplace, living_room)

///////////////////////////////////////////////////////////////////////////////

FURNITURE(ornaments, "ornaments", R"(
There are various small figurines of china and brass.  There is something
faintly disturbing about them, but you can't quite put your finger on it.  Best
leave these alone.
)")

PLACE(ornaments, living_room)

///////////////////////////////////////////////////////////////////////////////

FURNITURE(picture, "picture", R"(
It is a picture of, you think, an 18th century rural setting.  A river winds
through the centre of the late afternoon scene.  On one bank, an unattended cart
seems to be stuck in the soft earth.  It is loaded with baskets of fruit,
possibly apples.  On the other bank, a field stretches to the horizon.

At the bottom of the picture is a title: "Ideas Of Sin".
)")

PLACE(picture, living_room)

///////////////////////////////////////////////////////////////////////////////

FURNITURE(shelves, "shelves", R"(
"shelves",
The wooden shelves are very dusty.  On each is an uncoordinated assortment of
small china and brass ornaments.
)")

PLACE(shelves, living_room)

///////////////////////////////////////////////////////////////////////////////

FURNITURE(sofa, "sofa", R"(
It is a very comfy sofa, but you don't feel like resting right now.
)")

PLACE(sofa, living_room)

////////////////////////////////////////////////////////////////////////////////

// not initially placed anywhere
ITEM(fragments, "vase fragments", R"(
Sharp fragments of varying shapes.
)")

USE(fragments, {
    if (patient != sofa) {
        return false;
    }

    gs.add_message(R"(
You slash away at the sofa with a sharp vase fragment, utterly ruining the
upholstery.

While attacking one cushion at the height of your frenzy, you hit something hard
and metallic buried within.

A brass key comes flying out, and lands on the floor!
    )");

    gs.set_description(sofa, R"(
The sofa is utterly ruined.
    )");

    gs.move_to_location(brass_key);
    return true;
})

///////////////////////////////////////////////////////////////////////////////
