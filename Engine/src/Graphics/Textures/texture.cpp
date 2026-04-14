#include <Graphics/Textures/texture.hpp>
#include <Graphics/Textures/textureManager.hpp>

Texture::Texture()
{
}

Texture::Texture (size_t idIn,
                  TextureManager* managerIn) : id (idIn),
                                               manager (managerIn)

{
}
int Texture::getX() const { return manager->getData (id)->bounds.x; }
int Texture::getY() const { return manager->getData (id)->bounds.x; }
int Texture::getHeight() const { return manager->getData (id)->bounds.w; }
int Texture::getWidth() const { return manager->getData (id)->bounds.h; }

float Texture::getU() const
{
    auto xPixels = static_cast<float> (manager->getData (id)->bounds.x);
    auto textureSizePixelW = static_cast<float> (manager->textureSize.x);

    return xPixels / textureSizePixelW;
}
float Texture::getV() const
{
    auto yPixels = static_cast<float> (manager->getData (id)->bounds.y);
    auto textureSizePixelH = static_cast<float> (manager->textureSize.y);

    return yPixels / textureSizePixelH;
}
float Texture::getW() const
{
    auto width = static_cast<float> (manager->getData (id)->bounds.w);
    auto textureSizePixelW = static_cast<float> (manager->textureSize.x);

    return (width) / textureSizePixelW;
}
float Texture::getH() const
{
    auto height = static_cast<float> (manager->getData (id)->bounds.h);
    auto textureSizePixelH = static_cast<float> (manager->textureSize.y);

    return (height) / textureSizePixelH;
}

glm::vec2 Texture::translatePixelToUV (int x, int y) const
{
    return {
        static_cast<float> (x) / static_cast<float> (manager->textureSize.x),
        static_cast<float> (y) / static_cast<float> (manager->textureSize.y)
    };
}

glm::vec4 Texture::getSubRectUV (int u, int v, int w, int h) const
{
    auto parentX = manager->getData (id)->bounds.x;
    auto parentY = manager->getData (id)->bounds.y;

    auto translatedX = parentX + u;
    auto translatedY = parentY + v;
    auto translatedW = w;
    auto translatedH = h;

    return {
        static_cast<float> (translatedX) / static_cast<float> (manager->textureSize.x),
        static_cast<float> (translatedY) / static_cast<float> (manager->textureSize.y),
        static_cast<float> (translatedW) / static_cast<float> (manager->textureSize.x),
        static_cast<float> (translatedH) / static_cast<float> (manager->textureSize.y)
    };
}