#pragma once
#include "graphicsContext.hpp"

struct SpriteData {
    float x,y;
    float rotation;
    float w,h;
    float paddingA, paddingB;
    float textureU, textureV, textureW,textureH;
    float r,g,b,a;
};

class SpriteBatch{
    public:
    SpriteBatch(GraphicsContext * context);
    

    private:
    SDL_GPUGraphicsPipeline * RenderPipeline;
    SDL_GPUSampler * sampler;
    SDL_GPUTexture * texture;

    SDL_GPUTransferBuffer* transferBuffer;
    SDL_GPUBuffer * dataBuffer;

    GraphicsContext * context;
};