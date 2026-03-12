#include <Engine/Utils/vectorTypes.hpp>
#include <rectpack2D/rect_structs.h>

Vec2::Vec2 (int xIn, int yIn) : x (xIn),
                                y (yIn)
{
}

Vec2::Vec2 (rectpack2D::rect_wh rect) : x (rect.w),
                                        y (rect.h)
{
}