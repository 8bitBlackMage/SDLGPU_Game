#pragma once
#include <Engine/Utils/vectorTypes.hpp>
#include <rectpack2D/rect_structs.h>
/// @brief Thin wrapper around SDL GPU Texture.

class TextureManager;

class Texture
{
public:
    Texture();
    const int getX() const;
    const int getY() const;
    const int getHeight() const;
    const int getWidth() const;

    const float getU() const;
    const float getV() const;
    const float getW() const;
    const float getH() const;

    const Vec2<float> translatePixelToUV (int x, int y) const;
    const Vec4<float> getSubRectUV (int u, int v, int w, int h) const;

private:
    friend class TextureManager;
    Texture (size_t idIn, TextureManager* managerIn);
    unsigned int id;
    TextureManager* manager;
};