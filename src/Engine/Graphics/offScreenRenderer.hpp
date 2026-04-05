#ifndef OFFSCREENRENDERER_HPP
#define OFFSCREENRENDERER_HPP

#include "Engine/Utils/vectorTypes.hpp"
#include "SDL3/SDL_gpu.h"
#include <Engine/Utils/matricies.hpp>
#include <SDL3/SDL.h>

class GraphicsContext;
struct FrameContext;

class OffScreenRenderer
{
public:
    OffScreenRenderer() = default;
    void init (GraphicsContext* context);

    void render (FrameContext* context, SDL_GPUTexture* renderTexture, Vec2<float> renderTextureSize, Matrix4x4 outputMatrix);

private:
    SDL_GPUGraphicsPipeline* RenderPipeline;
    SDL_GPUSampler* sampler;

    SDL_GPUTransferBuffer* transferBuffer;
    SDL_GPUBuffer* vertexBuffer;
    SDL_GPUBuffer* indexBuffer;
};

#endif