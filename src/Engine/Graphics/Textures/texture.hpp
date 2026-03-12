#pragma once
#include <rectpack2D/rect_structs.h>
/// @brief Thin wrapper around SDL GPU Texture.

class TextureManager;

class Texture
{
public:
    Texture();
    const int getHeight() const;
    const int getWidth() const;

    const float getU() const;
    const float getV() const;
    const float getW() const;
    const float getH() const;

private:
    friend class TextureManager;
    Texture (size_t idIn, TextureManager* managerIn);
    size_t id;
    TextureManager* manager;
};