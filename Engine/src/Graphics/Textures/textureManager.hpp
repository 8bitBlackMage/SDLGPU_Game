#pragma once
#include <Graphics/Textures/texture.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <SDL3/SDL.h>
#include <map>
#include <string>
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

    Texture getTextureForFile (std::string filename);

    ///@brief gets the raw handle of the texture pointer, this should only be used when binding to a pipeline.
    inline SDL_GPUTexture* getRawGPUTexture() { return texture; }

    void debugView();

private:
    glm::vec2 solve();

    glm::vec4 pixelToUV (int x, int y, int w, int h);

    ///@brief inner texture class that actually stores data pertaining to textures.
    class TextureData
    {
    public:
        TextureData (std::string file, size_t idHandle) : fileName (file), id (idHandle)
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
        size_t id;
        rectpack2D::rect_xywhf bounds;
    };
    std::map<size_t, size_t> hashedFileNames;
    std::vector<TextureData> textures;

    TextureData* getData (size_t id) { return &textures[hashedFileNames[id]]; }

    glm::vec2 textureSize;
    SDL_GPUTexture* texture;

    int zoomFactor = 4;

    bool uploading;
};
