#pragma once
#include <rectpack2D/rect_structs.h>

class Vec2
{
public:
    int x, y;

    Vec2 (int xIn = 0, int yIn = 0);
    Vec2 (rectpack2D::rect_wh rect);
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