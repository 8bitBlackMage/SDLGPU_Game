#pragma once
#include "spriteBatch.hpp"

class Sprite
{
public:
    Sprite();

    struct SpriteData
    {
        float x, y, z;
        float rotation;
        float w, h;
        float paddingA, paddingB;
        float textureU, textureV, textureW, textureH;
        float r, g, b, a;
    };
    SpriteData inner;
    friend class SpriteBatch;
};