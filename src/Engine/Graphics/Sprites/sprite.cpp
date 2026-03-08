#include "sprite.hpp"

Sprite::Sprite()
{
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