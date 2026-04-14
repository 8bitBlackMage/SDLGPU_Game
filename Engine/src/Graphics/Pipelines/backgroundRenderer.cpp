#include "SDL3/SDL_gpu.h"
#include "SDL3/SDL_pixels.h"
#include <Graphics/Pipelines/backgroundRenderer.hpp>
#include <Graphics/graphicsContext.hpp>
#include <Utils/logger.hpp>

void BackgroundRenderer::init (GraphicsContext* context)
{
    if (context == nullptr)
    {
        return;
        Logger::log ("Invalid GPU Context provided");
    }

    SDL_GPUShader* vert = context->loadShader ("backgroundrenderer.vert", 0, 1, 0, 0);
    SDL_GPUShader* frag = context->loadShader ("backgroundrenderer.frag", 0, 0, 0, 0);

    auto colourTargetDesc = SDL_GPUColorTargetDescription {
        .format = SDL_GetGPUSwapchainTextureFormat (context->getDevice(), context->getWindow()),
        .blend_state = {

            .enable_blend = false,
            .color_blend_op = SDL_GPU_BLENDOP_REVERSE_SUBTRACT,
            .alpha_blend_op = SDL_GPU_BLENDOP_ADD,
            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
        }
    };

    auto createInfo = SDL_GPUGraphicsPipelineCreateInfo { .target_info = SDL_GPUGraphicsPipelineTargetInfo {
                                                              .num_color_targets = 1,
                                                              .color_target_descriptions = &colourTargetDesc,

                                                          },
                                                          .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
                                                          .vertex_shader = vert,
                                                          .fragment_shader = frag };

    RenderPipeline = SDL_CreateGPUGraphicsPipeline (context->getDevice(), &createInfo);

    SDL_ReleaseGPUShader (context->getDevice(), vert);
    SDL_ReleaseGPUShader (context->getDevice(), frag);

    setBackgroundColour ({ 132, 132, 130, 255 });
}

void BackgroundRenderer::render (FrameContext* frameContext)
{
    if (! frameContext->device)
    {
        return;
    }

    if (frameContext->commandBuffer == nullptr)
    {
        Logger::log ("Failed to acquire command buffer", SDL_GetError());
    }

    if (frameContext->swapchainTexture == nullptr)
    {
        return;
    }

    auto colourTargetInfo = SDL_GPUColorTargetInfo {
        .texture = frameContext->swapchainTexture,
        .cycle = false,
        .load_op = SDL_GPU_LOADOP_LOAD,
        .store_op = SDL_GPU_STOREOP_STORE,
        .clear_color { 0, 0, 0, 0 }
    };

    SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass (
        frameContext->commandBuffer,
        &colourTargetInfo,
        1,
        nullptr);

    SDL_BindGPUGraphicsPipeline (renderPass, RenderPipeline);
    SDL_SetGPUViewport (renderPass, &frameContext->viewport);

    SDL_PushGPUVertexUniformData (
        frameContext->commandBuffer,
        0,
        &backgroundColour,
        sizeof (SDL_FColor));
    SDL_DrawGPUPrimitives (
        renderPass,
        6,
        1,
        0,
        0);

    SDL_EndGPURenderPass (renderPass);
}

void BackgroundRenderer::setBackgroundColour (SDL_Color colour)
{
    backgroundColour.r = (float) colour.r / 255.f;
    backgroundColour.g = (float) colour.b / 255.f;
    backgroundColour.b = (float) colour.b / 255.f;
    backgroundColour.a = (float) colour.a / 255.f;
}