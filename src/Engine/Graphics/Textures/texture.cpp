#include <Engine/Graphics/Textures/texture.hpp>
#include <Engine/Graphics/Textures/textureManager.hpp>
#include <Engine/Utils/vectorTypes.hpp>

Texture::Texture()
{
}

Texture::Texture (size_t idIn,
                  TextureManager* managerIn) : id (idIn),
                                               manager (managerIn)

{
}

const int Texture::getHeight() const { return manager->textures[id].bounds.w; }
const int Texture::getWidth() const { return manager->textures[id].bounds.h; }

const float Texture::getU() const
{
    auto xPixels = static_cast<float> (manager->textures[id].bounds.x);
    auto textureSizePixelW = static_cast<float> (manager->textureSize.x);

    return xPixels / textureSizePixelW;
}
const float Texture::getV() const
{
    auto yPixels = static_cast<float> (manager->textures[id].bounds.y);
    auto textureSizePixelH = static_cast<float> (manager->textureSize.y);

    return yPixels / textureSizePixelH;
}
const float Texture::getW() const
{
    auto xPixels = static_cast<float> (manager->textures[id].bounds.x);
    auto width = static_cast<float> (manager->textures[id].bounds.w);
    auto textureSizePixelW = static_cast<float> (manager->textureSize.x);

    return (xPixels + width) / textureSizePixelW;
}
const float Texture::getH() const
{
    auto yPixels = static_cast<float> (manager->textures[id].bounds.y);
    auto height = static_cast<float> (manager->textures[id].bounds.h);
    auto textureSizePixelH = static_cast<float> (manager->textureSize.y);

    return (yPixels + height) / textureSizePixelH;
}

const Vec2<float> Texture::translatePixelToUV (int x, int y) const
{
    return {
        static_cast<float> (x) / static_cast<float> (manager->textureSize.x),
        static_cast<float> (y) / static_cast<float> (manager->textureSize.y)
    };
}