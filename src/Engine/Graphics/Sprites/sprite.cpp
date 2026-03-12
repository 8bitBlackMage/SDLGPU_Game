#include "Engine/Graphics/Textures/texture.hpp"
#include <Engine/Graphics/Sprites/sprite.hpp>

Sprite::Sprite() {}

Sprite::Sprite (Texture t, int x, int y)
{
    setTexture (t);
}
Sprite::Sprite (Texture t, int x, int y, int u, int v, int w, int h)
{
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
void Sprite::setTextureFromPortion (Texture texture, int u, int v, int w, int h)
{
}