#pragma once
#include <Graphics/Pipelines/spriteBatch.hpp>
#include <Graphics/Textures/texture.hpp>

/// @brief Sprite class, for use with the SpriteBatch renderer.
class Sprite
{
public:
    Sprite();
    Sprite (Texture t, int x = 0, int y = 0);
    Sprite (Texture t, int x, int y, int width, int height, int u, int v, int w, int h);

    void setTexture (Texture texture);
    void setTextureFromPortion (Texture texture,
                                int width,
                                int height,
                                int u,
                                int v,
                                int w,
                                int h);

    friend class SpriteBatch;

private:
    struct SpriteData
    {
        float x = 0;
        float y = 0;
        float z = 0;
        float rotation = 0;
        float w = 0;
        float h = 0;
        float paddingA = 0;
        float paddingB = 0;
        float textureU = 0;
        float textureV = 0;
        float textureW = 0;
        float textureH = 0;
        float r = 1;
        float g = 1;
        float b = 1;
        float a = 1;
    };
    SpriteData inner;

    Texture texture;
};