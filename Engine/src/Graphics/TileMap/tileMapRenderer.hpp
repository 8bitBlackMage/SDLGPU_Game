#pragma once

#include <Graphics/Textures/texture.hpp>
#include <Graphics/graphicsContext.hpp>

#include <LDtkLoader/Level.hpp>
#include <LDtkLoader/Project.hpp>
#include <SDL3/SDL.h>
#include <vector>
class TileMapRenderer
{
public:
    TileMapRenderer();
    void init (GraphicsContext* context);
    void preLoadTextures (const ldtk::Project& project, GraphicsContext* context);
    void loadTileMap (const ldtk::Level& level, GraphicsContext* context);

    void draw (FrameContext* frameContext);

private:
    //internal data type for tile data.
    struct Tile
    {
        float x = 0;
        float y = 0;
        float z = 0;
        float padding;
        float w = 0;
        float h = 0;
        float padding1;
        float padding2;
        float textureU = 0;
        float textureV = 0;
        float textureW = 0;
        float textureH = 0;
        float r = 1;
        float g = 1;
        float b = 1;
        float a = 1;
    };

    std::vector<Tile> tileData;

    SDL_GPUGraphicsPipeline* RenderPipeline;
    SDL_GPUSampler* sampler;

    SDL_GPUTransferBuffer* transferBuffer;
    SDL_GPUBuffer* dataBuffer;
};