#pragma once
#include "graphicsContext.hpp"

struct SpriteData
{
    float x, y, z;
    float rotation;
    float w, h;
    float paddingA, paddingB;
    float textureU, textureV, textureW, textureH;
    float r, g, b, a;
};

class SpriteBatch
{
public:
    SpriteBatch();

    void init (GraphicsContext* context);

    void draw (FrameContext* context);

    void drawDebugInfo();

private:
    SDL_GPUGraphicsPipeline* RenderPipeline;
    SDL_GPUSampler* sampler;
    Texture texture;

    SDL_GPUTransferBuffer* transferBuffer;
    SDL_GPUBuffer* dataBuffer;

    GraphicsContext* context;

    int spritesToDraw = 256;
    float renderTime;
};