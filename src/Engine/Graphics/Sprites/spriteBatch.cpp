#include "SDL3/SDL_gpu.h"
#include <Engine/Graphics/Sprites/spriteBatch.hpp>

#include <Engine/Utils/logger.hpp>
#include <Engine/Utils/matricies.hpp>

const size_t SPRITE_COUNT = 65536;

SpriteBatch::SpriteBatch()
{
}

void SpriteBatch::init (GraphicsContext* context)
{
    if (context == nullptr)
    {
        return;
        Logger::log ("Invalid GPU Context provided");
    }

    SDL_GPUShader* vert = context->loadShader ("spritebatch.vert", 0, 1, 1, 0);
    SDL_GPUShader* frag = context->loadShader ("spritebatch.frag", 1, 0, 0, 0);

    auto createInfo = (SDL_GPUGraphicsPipelineCreateInfo) {
        .target_info = (SDL_GPUGraphicsPipelineTargetInfo) {
            .num_color_targets = 1,
            .color_target_descriptions = (SDL_GPUColorTargetDescription[]) { { .format = SDL_GetGPUSwapchainTextureFormat (context->getDevice(), context->getWindow()),
                                                                               .blend_state = {
                                                                                   .enable_blend = true,
                                                                                   .color_blend_op = SDL_GPU_BLENDOP_ADD,
                                                                                   .alpha_blend_op = SDL_GPU_BLENDOP_ADD,
                                                                                   .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
                                                                                   .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                                                                                   .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
                                                                                   .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                                                                               } } } },
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .vertex_shader = vert,
        .fragment_shader = frag
    };

    RenderPipeline = SDL_CreateGPUGraphicsPipeline (context->getDevice(), &createInfo);

    SDL_ReleaseGPUShader (context->getDevice(), vert);
    SDL_ReleaseGPUShader (context->getDevice(), frag);

    auto samplerCreateInfo = (SDL_GPUSamplerCreateInfo) {
        .min_filter = SDL_GPU_FILTER_NEAREST,
        .max_anisotropy = SDL_GPU_FILTER_NEAREST,
        .mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
        .address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
        .address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
        .address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
    };

    sampler = SDL_CreateGPUSampler (context->getDevice(), &samplerCreateInfo);

    auto transferBufferCreateInfo = (SDL_GPUTransferBufferCreateInfo) {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = SPRITE_COUNT * sizeof (Sprite::SpriteData)
    };

    transferBuffer = SDL_CreateGPUTransferBuffer (context->getDevice(), &transferBufferCreateInfo);
    if (transferBuffer == nullptr)
    {
        Logger::log ("failed to create transfer buffer", SDL_GetError());
    }

    auto bufferCreateInfo = (SDL_GPUBufferCreateInfo) {
        .usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
        .size = SPRITE_COUNT * sizeof (Sprite::SpriteData)
    };

    dataBuffer = SDL_CreateGPUBuffer (context->getDevice(), &bufferCreateInfo);
}

void SpriteBatch::draw (Sprite* sprite)
{
    if (sprite != nullptr)
    {
        spriteQueue.push (sprite);
    }
}

void SpriteBatch::render (FrameContext* frameContext)
{
    if (frameContext->textureManager->getRawGPUTexture() == nullptr)
    {
        return;
    }

    float timeAtStart = SDL_GetTicksNS();

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

    Sprite::SpriteData* data = (Sprite::SpriteData*) SDL_MapGPUTransferBuffer (frameContext->device,
                                                                               transferBuffer,
                                                                               true);

    int i = 0;
    for (; ! spriteQueue.empty(); spriteQueue.pop())
    {
        data[i] = spriteQueue.front()->inner;
        i++;
    }
    SDL_UnmapGPUTransferBuffer (frameContext->device, transferBuffer);

    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass (frameContext->commandBuffer);

    auto transferBufferLocation = (SDL_GPUTransferBufferLocation) {
        .offset = 0,
        .transfer_buffer = transferBuffer
    };
    auto bufferRegion = (SDL_GPUBufferRegion) {
        .buffer = dataBuffer,
        .offset = 0,
        .size = SPRITE_COUNT * sizeof (Sprite::SpriteData)
    };

    SDL_UploadToGPUBuffer (
        copyPass,
        &transferBufferLocation,
        &bufferRegion,
        true);

    SDL_EndGPUCopyPass (copyPass);

    auto colourTargetInfo = (SDL_GPUColorTargetInfo) {
        .texture = frameContext->swapchainTexture,
        .cycle = false,
        .load_op = SDL_GPU_LOADOP_LOAD,
        .store_op = SDL_GPU_STOREOP_STORE,
        .clear_color { 0, 0, 0, 1 }
    };

    SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass (
        frameContext->commandBuffer,
        &colourTargetInfo,
        1,
        nullptr);

    SDL_BindGPUGraphicsPipeline (renderPass, RenderPipeline);
    SDL_BindGPUVertexStorageBuffers (
        renderPass,
        0,
        &dataBuffer,
        1);

    auto samplerBinding = (SDL_GPUTextureSamplerBinding) {
        .texture = frameContext->textureManager->getRawGPUTexture(),
        .sampler = sampler
    };

    SDL_BindGPUFragmentSamplers (
        renderPass,
        0,
        &samplerBinding,
        1);

    SDL_PushGPUVertexUniformData (
        frameContext->commandBuffer,
        0,
        &frameContext->cameraMatrix,
        sizeof (Matrix4x4));
    SDL_DrawGPUPrimitives (
        renderPass,
        spritesToDraw * 6,
        1,
        0,
        0);

    SDL_EndGPURenderPass (renderPass);
    float timeAtEnd = SDL_GetTicksNS();
}

void SpriteBatch::debugView()
{
    ImGui::Begin ("SpriteBatch");
    {
        ImGui::SliderInt ("Sprites", &spritesToDraw, 0, 428);
        ImGui::End();
    }
}
