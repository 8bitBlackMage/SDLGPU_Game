#include <Engine/Graphics/Sprites/sprite.hpp>

Sprite::Sprite()
{
    SDL_srand (SDL_GetPerformanceCounter());
    inner = (SpriteData) {
        .x = 32,
        .y = 32,
        .z = 0,
        .rotation = SDL_randf() * SDL_PI_F * 2,
        .w = 32,
        .h = 32,
        .textureU = 0.5f,
        .textureV = 0.5f,
        .textureW = 0.5f,
        .textureH = 0.5f,
        .r = 1.0f,
        .g = 1.0f,
        .b = 1.0f,
        .a = 1.0f,
    };
}

void Sprite::setX (float newX) noexcept
{
    inner.x = newX;
}

float Sprite::getX() const noexcept
{
    return inner.x;
}

void Sprite::setY (float newY) noexcept
{
    inner.y = newY;
}

float Sprite::getY() const noexcept
{
    return inner.y;
}

void Sprite::setPostion (float x, float y) noexcept
{
    inner.x = x;
    inner.y = y;
}

SDL_FRect Sprite::getTextureBounds() const noexcept
{
    return SDL_FRect { inner.textureU, inner.textureV, inner.textureW, inner.textureH };
}

void Sprite::setTextureBounds (SDL_FRect rect) noexcept
{
    inner.textureU = rect.x;
    inner.textureV = rect.y;
    inner.textureW = rect.w;
    inner.textureH = rect.h;
}

void Sprite::setTextureBounds (float x, float y, float w, float h) noexcept
{
    inner.textureU = x;
    inner.textureV = y;
    inner.textureW = w;
    inner.textureH = h;
}