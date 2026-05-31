#include "Graphics/graphicsContext.hpp"
#include "SDL3/SDL_blendmode.h"
#include "SDL3/SDL_gpu.h"
#include "glm/ext/vector_float2.hpp"
#include <Graphics/Pipelines/lineRenderer.hpp>
#include <Graphics/Structs/line.hpp>
#include <Utils/logger.hpp>

const size_t LINE_COUNT = 65536;

void LineRenderer::init (GraphicsContext* context)
{
    if (context == nullptr)
    {
        return;
        Logger::log ("Invalid GPU Context provided");
    }

    SDL_GPUShader* vert = context->loadShader ("lineRenderer.vert", 0, 1, 1, 0);
    SDL_GPUShader* frag = context->loadShader ("lineRenderer.frag", 0, 0, 0, 0);

    auto colourTargetDesc = SDL_GPUColorTargetDescription {
        .format = SDL_GetGPUSwapchainTextureFormat (context->getDevice(), context->getWindow()),
        .blend_state = {

            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            .color_blend_op = SDL_GPU_BLENDOP_REVERSE_SUBTRACT,
            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            .alpha_blend_op = SDL_GPU_BLENDOP_ADD,
            .enable_blend = false,
        }
    };

    auto createInfo = SDL_GPUGraphicsPipelineCreateInfo { .vertex_shader = vert,
                                                          .fragment_shader = frag,
                                                          .primitive_type = SDL_GPU_PRIMITIVETYPE_LINELIST,
                                                          .target_info = SDL_GPUGraphicsPipelineTargetInfo {
                                                              .color_target_descriptions = &colourTargetDesc,
                                                              .num_color_targets = 1,

                                                          } };

    RenderPipeline = SDL_CreateGPUGraphicsPipeline (context->getDevice(), &createInfo);

    SDL_ReleaseGPUShader (context->getDevice(), vert);
    SDL_ReleaseGPUShader (context->getDevice(), frag);

    auto transferBufferCreateInfo = SDL_GPUTransferBufferCreateInfo {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = LINE_COUNT * sizeof (glm::vec2) * 2
    };

    transferBuffer = SDL_CreateGPUTransferBuffer (context->getDevice(), &transferBufferCreateInfo);
    if (transferBuffer == nullptr)
    {
        Logger::log ("failed to create transfer buffer", SDL_GetError());
    }

    auto bufferCreateInfo = SDL_GPUBufferCreateInfo {
        .usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
        .size = LINE_COUNT * sizeof (glm::vec2) * 2
    };

    dataBuffer = SDL_CreateGPUBuffer (context->getDevice(), &bufferCreateInfo);
}

void LineRenderer::draw (Line* line)
{
    lineQueue.push (line);
}

void LineRenderer::render (FrameContext* frameContext)
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

    glm::vec2* data = (glm::vec2*) SDL_MapGPUTransferBuffer (frameContext->device, transferBuffer, true);
    size_t verts = 0;
    for (; ! lineQueue.empty(); lineQueue.pop())
    {
        data[verts] = lineQueue.front()->start;
        verts++;
        data[verts] = lineQueue.front()->end;
        verts++;
    }
    SDL_UnmapGPUTransferBuffer (frameContext->device, transferBuffer);

    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass (frameContext->commandBuffer);

    auto transferBufferLocation = SDL_GPUTransferBufferLocation {
        .transfer_buffer = transferBuffer,
        .offset = 0
    };
    auto bufferRegion = SDL_GPUBufferRegion {
        .buffer = dataBuffer,
        .offset = 0,
        .size = LINE_COUNT * sizeof (glm::vec2)
    };

    SDL_UploadToGPUBuffer (
        copyPass,
        &transferBufferLocation,
        &bufferRegion,
        true);

    SDL_EndGPUCopyPass (copyPass);

    auto colourTargetInfo = SDL_GPUColorTargetInfo {
        .texture = frameContext->swapchainTexture,
        .clear_color { 0, 0, 0, 0 },
        .load_op = SDL_GPU_LOADOP_LOAD,
        .store_op = SDL_GPU_STOREOP_STORE,
        .cycle = false
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

    SDL_SetGPUViewport (renderPass, &frameContext->viewport);

    SDL_PushGPUVertexUniformData (
        frameContext->commandBuffer,
        0,
        &frameContext->cameraMatrix,
        sizeof (glm::mat4x4));
    SDL_DrawGPUPrimitives (
        renderPass,
        verts + 1,
        1,
        0,
        0);

    SDL_EndGPURenderPass (renderPass);
}
