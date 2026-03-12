#pragma once
#include <Engine/Graphics/Textures/texture.hpp>
#include <Engine/Utils/vectorTypes.hpp>
#include <SDL3/SDL.h>
#include <vector>

class GraphicsContext;

///@brief Object to handle optimised loading of textures into GPU programming.
class TextureManager
{
public:
    friend class Texture;

    TextureManager();

    ///@brief Call at the start of a scene to begin loading.
    void beginBatchUpload (GraphicsContext* context);

    ///@brief Append texture to upload list. texture pointer will not have valid info until endBatchUpload is called.
    ///@param filename name of img file to upload
    Texture loadTexture (std::string filename);

    ///@brief Call at the end of a scene to finalize and up
    void endBatchUpload (GraphicsContext* context);

    ///@brief gets the raw handle of the texture pointer, this should only be used when binding to a pipeline.
    inline SDL_GPUTexture* getRawGPUTexture() { return texture; }

    void debugView();

private:
    Vec2<int> solve();

    Vec4<float> pixelToUV (int x, int y, int w, int h);

    ///@brief inner texture class that actually stores data pertaining to textures.
    class TextureData
    {
    public:
        TextureData (std::string file) : fileName (file)
        {
        }

        auto& get_rect()
        {
            return bounds;
        }

        const auto& get_rect() const
        {
            return bounds;
        }

        std::string fileName;
        rectpack2D::rect_xywhf bounds;
    };

    std::vector<TextureData> textures;

    Vec2<int> textureSize;
    SDL_GPUTexture* texture;

    int zoomFactor = 4;
};