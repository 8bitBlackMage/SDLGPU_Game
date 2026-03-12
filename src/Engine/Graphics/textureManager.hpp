#pragma once
#include "Engine/Utils/vectorTypes.hpp"

#include <SDL3/SDL_gpu.h>
#include <rectpack2D/rect_structs.h>
#include <vector>
class GraphicsContext;

/// @brief Thin wrapper around SDL GPU Texture.
struct Texture
{
    SDL_GPUTexture* texture;
    size_t id;
    int x, y;
    int width, height;

    rectpack2D::rect_xywhf rect;
};

///@brief Object to handle optimised loading of textures into GPU programming.
class TextureManager
{
public:
    TextureManager();

    ///@brief Call at the start of a scene to begin loading.
    void beginBatchUpload (GraphicsContext* context);

    ///@brief Append texture to upload list. texture pointer will not have valid info until endBatchUpload is called.
    ///@param filename name of img file to upload
    Texture loadTexture (std::string filename);

    ///@brief Call at the end of a scene to finalize and up
    void endBatchUpload (GraphicsContext* context);

    void debugView();

private:
    Vec2 solve();

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

    Vec2 textureSize;
    SDL_GPUTexture* texture;

    int zoomFactor = 4;
};