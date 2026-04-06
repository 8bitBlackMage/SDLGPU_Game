#ifndef OFFSCREENRENDERER_HPP
#define OFFSCREENRENDERER_HPP

#include <Utils/vectorTypes.hpp>

#include "glm/ext/matrix_float4x4.hpp"
#include <SDL3/SDL.h>
#include <glm/mat4x4.hpp>

class GraphicsContext;
struct FrameContext;

class OffScreenRenderer
{
public:
    OffScreenRenderer() = default;
    void init (GraphicsContext* context);

    void render (FrameContext* context, SDL_GPUTexture* renderTexture, Vec2<float> renderTextureSize, glm::mat4x4 outputMatrix);

private:
    SDL_GPUGraphicsPipeline* RenderPipeline;
    SDL_GPUSampler* sampler;

    SDL_GPUTransferBuffer* transferBuffer;
    SDL_GPUBuffer* vertexBuffer;
    SDL_GPUBuffer* indexBuffer;
};

#endif