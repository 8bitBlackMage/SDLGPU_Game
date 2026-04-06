#pragma once
#include <Utils/vectorTypes.hpp>
#include <rectpack2D/rect_structs.h>
/// @brief Thin wrapper around SDL GPU Texture.

class TextureManager;

class Texture
{
public:
    Texture();
    int getX() const;
    int getY() const;
    int getHeight() const;
    int getWidth() const;

    float getU() const;
    float getV() const;
    float getW() const;
    float getH() const;

    Vec2<float> translatePixelToUV (int x, int y) const;
    Vec4<float> getSubRectUV (int u, int v, int w, int h) const;

private:
    friend class TextureManager;
    Texture (size_t idIn, TextureManager* managerIn);
    size_t id;
    TextureManager* manager;
};
