#pragma once
#include <Engine/Graphics/Sprites/spriteBatch.hpp>

/// @brief Sprite class, for use with the SpriteBatch renderer.
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

    void setX (float newX) noexcept;
    float getX() const noexcept;

    void setY (float newY) noexcept;
    float getY() const noexcept;

    void setPostion (float x, float y) noexcept;

    SDL_FRect getTextureBounds() const noexcept;

    void setTextureBounds (SDL_FRect rect) noexcept;

    void setTextureBounds (float x, float y, float w, float h) noexcept;
};