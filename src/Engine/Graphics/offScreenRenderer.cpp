#include "Engine/Utils/vectorTypes.hpp"
#include "SDL3/SDL_gpu.h"
#include "SDL3/SDL_stdinc.h"
#include <Engine/Graphics/offScreenRenderer.hpp>
#include <Engine/Utils/logger.hpp>
#include <Graphics/graphicsContext.hpp>
#include <cstddef>

void OffScreenRenderer::init (GraphicsContext* context)
{
    if (context == nullptr)
    {
        return;
        Logger::log ("Invalid GPU Context provided");
    }

    SDL_GPUShader* vert = context->loadShader (
        "offscreen.vert",
        0,
        1,
        0,
        0);
    SDL_GPUShader* frag = context->loadShader (
        "offscreen.frag",
        1,
        0,
        0,
        0);

    auto colourTargetDesc = SDL_GPUColorTargetDescription {
        .format = SDL_GetGPUSwapchainTextureFormat (context->getDevice(), context->getWindow()),
        .blend_state = {

            .enable_blend = true,
            .color_blend_op = SDL_GPU_BLENDOP_ADD,
            .alpha_blend_op = SDL_GPU_BLENDOP_ADD,
            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
        }
    };

    auto vertexBufferDesc = SDL_GPUVertexBufferDescription {
        .slot = 0,
        .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
        .instance_step_rate = 0,
        .pitch = sizeof (Vec4<float>)
    };

    SDL_GPUVertexAttribute vertexAttributes[2];

    vertexAttributes[0] = SDL_GPUVertexAttribute {
        .buffer_slot = 0,
        .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
        .location = 0,
        .offset = 0,
    };
    vertexAttributes[1] = SDL_GPUVertexAttribute {
        .buffer_slot = 0,
        .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
        .location = 1,
        .offset = sizeof (float) * 2,
    };

    auto vertexInputState = SDL_GPUVertexInputState {
        .num_vertex_buffers = 1,
        .vertex_buffer_descriptions = &vertexBufferDesc,
        .num_vertex_attributes = 2,
        .vertex_attributes = vertexAttributes
    };

    auto createInfo = SDL_GPUGraphicsPipelineCreateInfo {
        .target_info = SDL_GPUGraphicsPipelineTargetInfo {
            .num_color_targets = 1,
            .color_target_descriptions = &colourTargetDesc,

        },
        .vertex_input_state = vertexInputState,
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .vertex_shader = vert,
        .fragment_shader = frag
    };
    RenderPipeline = SDL_CreateGPUGraphicsPipeline (context->getDevice(), &createInfo);
    if (RenderPipeline == nullptr)
    {
        Logger::log (SDL_GetError());
    }
    SDL_ReleaseGPUShader (context->getDevice(), vert);
    SDL_ReleaseGPUShader (context->getDevice(), frag);

    auto samplerCreateInfo = SDL_GPUSamplerCreateInfo {
        .min_filter = SDL_GPU_FILTER_NEAREST,
        .max_anisotropy = SDL_GPU_FILTER_NEAREST,
        .mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
        .address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
        .address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
        .address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
    };

    sampler = SDL_CreateGPUSampler (context->getDevice(), &samplerCreateInfo);

    auto vertexBufferCreateInfo = SDL_GPUBufferCreateInfo {
        .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
        .size = sizeof (Vec4<float>) * 4
    };

    vertexBuffer = SDL_CreateGPUBuffer (context->getDevice(), &vertexBufferCreateInfo);

    auto indexBufferCreateinfo = SDL_GPUBufferCreateInfo {
        .usage = SDL_GPU_BUFFERUSAGE_INDEX,
        .size = sizeof (Uint16) * 6
    };

    indexBuffer = SDL_CreateGPUBuffer (context->getDevice(), &indexBufferCreateinfo);

    auto transferBufferCreateInfo = SDL_GPUTransferBufferCreateInfo {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = sizeof (Vec4<float>) * 4 + sizeof (Uint16) * 6
    };

    transferBuffer = SDL_CreateGPUTransferBuffer (context->getDevice(), &transferBufferCreateInfo);
    if (transferBuffer == nullptr)
    {
        Logger::log ("failed to create transfer buffer", SDL_GetError());
    }
}

void OffScreenRenderer::render (FrameContext* frameContext, SDL_GPUTexture* renderTexture, Vec2<float> renderTextureSize, Matrix4x4 outputMatrix)
{
    if (renderTexture == nullptr)
    {
        return;
    }

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

    int winWidth, winHeight;

    SDL_GetWindowSize (frameContext->window, &winWidth, &winHeight);

    float floatWinWidth = (float) winWidth;
    float floatWinHeight = (float) winHeight;

    float wScale = floatWinWidth / renderTextureSize.x;
    float hScale = floatWinHeight / renderTextureSize.y;

    float scaleFactor = floor (std::min (wScale, hScale));

    float xOffset = (floatWinWidth - renderTextureSize.x * scaleFactor) / 2;
    float yOffset = (floatWinHeight - renderTextureSize.y * scaleFactor) / 2;
    float scaledWidth = renderTextureSize.x * scaleFactor;
    float scaledHeight = renderTextureSize.y * scaleFactor;

    auto transferData = (Vec4<float>*) SDL_MapGPUTransferBuffer (
        frameContext->device,
        transferBuffer,
        false);

    transferData[0] = { xOffset, yOffset, 0.0f, 0.0f };
    transferData[1] = { xOffset + scaledWidth, yOffset, 1.0f, 0.0f };
    transferData[2] = { xOffset, yOffset + scaledHeight, 0.0f, 1.0f };
    transferData[3] = { xOffset + scaledWidth, yOffset + scaledHeight, 1.0f, 1.0f };

    Uint16* indexData = (Uint16*) &transferData[4];
    indexData[0] = 0;
    indexData[1] = 1;
    indexData[2] = 2;
    indexData[3] = 3;
    indexData[4] = 2;
    indexData[5] = 1;

    SDL_UnmapGPUTransferBuffer (frameContext->device, transferBuffer);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass (frameContext->commandBuffer);

    auto vertTransferLocation = SDL_GPUTransferBufferLocation {
        .transfer_buffer = transferBuffer,
        .offset = 0,
    };

    auto vertBufferRegion = SDL_GPUBufferRegion {
        .buffer = vertexBuffer,
        .offset = 0,
        .size = sizeof (Vec4<float>) * 4
    };

    SDL_UploadToGPUBuffer (copyPass,
                           &vertTransferLocation,
                           &vertBufferRegion,
                           false);

    auto indexTransferLocation = SDL_GPUTransferBufferLocation {
        .transfer_buffer = transferBuffer,
        .offset = sizeof (Vec4<float>) * 4,
    };
    auto indexBufferRegion = SDL_GPUBufferRegion {
        .buffer = indexBuffer,
        .offset = 0,
        .size = sizeof (Uint16) * 6,
    };

    SDL_UploadToGPUBuffer (copyPass, &indexTransferLocation, &indexBufferRegion, false);
    SDL_EndGPUCopyPass (copyPass);

    auto colourTargetInfo = SDL_GPUColorTargetInfo { .texture = frameContext->swapchainTexture, .cycle = false, .load_op = SDL_GPU_LOADOP_LOAD, .store_op = SDL_GPU_STOREOP_STORE, .clear_color { 0, 0, 0, 1 } };

    SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass (
        frameContext->commandBuffer,
        &colourTargetInfo,
        1,
        nullptr);

    SDL_BindGPUGraphicsPipeline (renderPass, RenderPipeline);
    auto samplerBinding = SDL_GPUTextureSamplerBinding {
        .texture = renderTexture,
        .sampler = sampler
    };

    auto vertexBinding = SDL_GPUBufferBinding { .buffer = vertexBuffer, .offset = 0 };
    auto indexBinding = SDL_GPUBufferBinding { .buffer = indexBuffer, .offset = 0 };

    SDL_BindGPUVertexBuffers (renderPass, 0, &vertexBinding, 1);
    SDL_BindGPUIndexBuffer (renderPass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_16BIT);

    SDL_BindGPUFragmentSamplers (
        renderPass,
        0,
        &samplerBinding,
        1);

    SDL_PushGPUVertexUniformData (
        frameContext->commandBuffer,
        0,
        &outputMatrix,
        sizeof (Matrix4x4));
    SDL_DrawGPUIndexedPrimitives (
        renderPass,
        6,
        1,
        0,
        0,
        0);

    SDL_EndGPURenderPass (renderPass);
}