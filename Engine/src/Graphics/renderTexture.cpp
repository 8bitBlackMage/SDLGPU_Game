#include "Graphics/graphicsContext.hpp"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_gpu.h"
#include "Utils/logger.hpp"
#include <Graphics/renderTexture.hpp>

RenderTexture::RenderTexture (GraphicsContext* context, float width, float height)
{
    init (context, width, height);
}

void RenderTexture::resize (GraphicsContext* context, float width, float height)
{
    SDL_ReleaseGPUTexture (context->getDevice(), target);
    init (context, width, height);
}

void RenderTexture::init (GraphicsContext* context, float width, float height)
{
    auto textureFormat = SDL_GetGPUSwapchainTextureFormat (context->getDevice(), context->getWindow());
    auto createInfo = SDL_GPUTextureCreateInfo {
        .type = SDL_GPU_TEXTURETYPE_2D,
        .format = textureFormat,
        .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
        .width = static_cast<Uint32> (width),
        .height = static_cast<Uint32> (height),
        .layer_count_or_depth = 1,
        .num_levels = 1,
        .sample_count = SDL_GPU_SAMPLECOUNT_1,
    };

    target = SDL_CreateGPUTexture (context->getDevice(), &createInfo);
    if (target == nullptr)
    {
        Logger::log ("failed to create render texture of width: ", width, "and height: ", height, "error:", SDL_GetError());
    }
    targetSize.x = width;
    targetSize.y = height;
}

SDL_GPUTexture* RenderTexture::getTexture()
{
    return target;
}
glm::vec2 RenderTexture::getSize()
{
    return targetSize;
}
