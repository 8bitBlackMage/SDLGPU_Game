#pragma once
#include <rectpack2D/rect_structs.h>

template <typename t>
class Vec2
{
public:
    t x, y;

    Vec2 (t xIn = 0, t yIn = 0) : x (xIn), y (yIn) {}
};

template <typename t>
class Vec4
{
public:
    t x, y, w, h;

    Vec4 (t xIn, t yIn, t wIn, t hIn) : x (xIn),
                                        y (yIn),
                                        w (wIn),
                                        h (hIn)
    {
    }
};