#ifndef OFFSCREENRENDERER_HPP
#define OFFSCREENRENDERER_HPP

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

    void render (FrameContext* context, SDL_GPUTexture* renderTexture, Matrix4x4 outputMatrix);

private:
    // std::vector<Vec2<float>> vertices;
    SDL_GPUGraphicsPipeline* RenderPipeline;
    SDL_GPUSampler* sampler;

    SDL_GPUTransferBuffer* transferBuffer;
    SDL_GPUBuffer* vertexBuffer;
    SDL_GPUBuffer* indexBuffer;
};

#endif