#include "Engine/Graphics/Textures/texture.hpp"
#include <Engine/Graphics/Sprites/sprite.hpp>

Sprite::Sprite() {}

Sprite::Sprite (Texture t, int x, int y)
{
    inner.x = x;
    inner.y = y;

    setTexture (t);
}
Sprite::Sprite (Texture t, int x, int y, int width, int height, int u, int v, int w, int h)
{
    inner.x = x;
    inner.y = y;

    setTextureFromPortion (t, width, height, u, v, w, h);
}

void Sprite::setTexture (Texture t)
{
    texture = t;
    inner.w = texture.getWidth();
    inner.h = texture.getHeight();
    inner.textureU = texture.getU();
    inner.textureV = texture.getV();
    inner.textureW = texture.getW();
    inner.textureH = texture.getH();
}
void Sprite::setTextureFromPortion (Texture t, int width, int height, int u, int v, int w, int h)
{
    texture = t;
    inner.w = width;
    inner.h = height;

    auto uv = texture.translatePixelToUV (u, v);
    auto wh = texture.translatePixelToUV (w, h);

    inner.textureU = uv.x;
    inner.textureV = uv.y;

    inner.textureW = wh.x;
    inner.textureH = wh.y;
}