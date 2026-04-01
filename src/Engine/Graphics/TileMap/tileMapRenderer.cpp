#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Layer.hpp"
#include "LDtkLoader/Level.hpp"
#include "LDtkLoader/Project.hpp"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_gpu.h"
#include <Engine/Graphics/TileMap/tileMapRenderer.hpp>
#include <Engine/Graphics/graphicsContext.hpp>
#include <Engine/Utils/logger.hpp>
#include <filesystem>

#include <Engine/Utils/matricies.hpp>
#include <cstddef>
#include <cstring>
#include <filesystem>

TileMapRenderer::TileMapRenderer()
{
}

void TileMapRenderer::init (GraphicsContext* context)
{
    if (context == nullptr)
    {
        Logger::log ("Invalid GPU Context provided");
        return;
    }

    SDL_GPUShader* vert = context->loadShader ("tilemap.vert", 0, 1, 1, 0);
    SDL_GPUShader* frag = context->loadShader ("tilemap.frag", 1, 0, 0, 0);

    auto desc = SDL_GPUColorTargetDescription {
        .format = SDL_GetGPUSwapchainTextureFormat (context->getDevice(), context->getWindow()),
        .blend_state = {
            .enable_blend = true,
            .color_blend_op = SDL_GPU_BLENDOP_ADD,
            .alpha_blend_op = SDL_GPU_BLENDOP_ADD,
            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
        },

    };

    auto createInfo = SDL_GPUGraphicsPipelineCreateInfo {
        .target_info = SDL_GPUGraphicsPipelineTargetInfo {
            .num_color_targets = 1,
            .color_target_descriptions = &desc,
        },
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .vertex_shader = vert,
        .fragment_shader = frag,
    };
    RenderPipeline = SDL_CreateGPUGraphicsPipeline (context->getDevice(), &createInfo);
    if (! RenderPipeline)
    {
        Logger::log ("Failed to create Render Pipeline: ", SDL_GetError());
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
}

void TileMapRenderer::preLoadTextures (const ldtk::Project& project, GraphicsContext* context)
{
    for (const auto& tileset : project.allTilesets())
    {
        auto fileName = std::filesystem::path (tileset.path).filename();
        context->getTextureManager()->loadTexture (fileName);
    }
}

void TileMapRenderer::loadTileMap (const ldtk::Level& level, GraphicsContext* context)
{
    for (auto layer = level.allLayers().rbegin(); layer != level.allLayers().rend(); layer++)
    {
        if (layer->getType() == ldtk::LayerType::Tiles)
        {
            std::filesystem::path path = layer->getTileset().path;
            auto texture = context->getTextureManager()->getTextureForFile (path.filename());
            for (auto& tile : layer->allTiles())
            {
                auto data = Tile {};
                auto texRect = tile.getTextureRect();
                auto uvwh = texture.getSubRectUV (texRect.x, texRect.y, texRect.width, texRect.height);

                data.x = tile.getPosition().x;
                data.y = tile.getPosition().y;

                data.w = tile.layer->getCellSize();
                data.h = tile.layer->getCellSize();

                data.textureU = uvwh.x;
                data.textureV = uvwh.y;
                data.textureW = uvwh.w;
                data.textureH = uvwh.h;

                data.a = tile.alpha;
                tileData.push_back (data);
            }
        }
    }

    const uint32_t size = static_cast<Uint32> (tileData.size() * sizeof (Tile));

    auto transferBufferCreateInfo = SDL_GPUTransferBufferCreateInfo {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = size
    };
    Logger::log ("attempting to create Transfer buffer of size:", size, "bytes");
    transferBuffer = SDL_CreateGPUTransferBuffer (context->getDevice(), &transferBufferCreateInfo);
    if (transferBuffer == nullptr)
    {
        Logger::log ("failed to create transfer buffer", SDL_GetError());
    }
    Logger::log ("attempting to create GPU buffer of size:", size, "bytes");
    auto bufferCreateInfo = SDL_GPUBufferCreateInfo {
        .usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
        .size = size
    };

    dataBuffer = SDL_CreateGPUBuffer (context->getDevice(), &bufferCreateInfo);
    if (dataBuffer == nullptr)
    {
        Logger::log ("failed to create data buffer", SDL_GetError());
    }

    Tile* data = (Tile*) SDL_MapGPUTransferBuffer (context->getDevice(),
                                                   transferBuffer,
                                                   false);

    std::memcpy (data, tileData.data(), sizeof (Tile) * tileData.size());

    SDL_UnmapGPUTransferBuffer (context->getDevice(), transferBuffer);

    auto commandBuffer = SDL_AcquireGPUCommandBuffer (context->getDevice());
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass (commandBuffer);

    auto transferBufferLocation = SDL_GPUTransferBufferLocation {
        .offset = 0,
        .transfer_buffer = transferBuffer
    };

    const auto tileSize = tileData.size() * sizeof (Tile);

    auto bufferRegion = SDL_GPUBufferRegion {
        .buffer = dataBuffer,
        .offset = 0,
        .size = static_cast<Uint32> (tileSize)
    };

    SDL_UploadToGPUBuffer (
        copyPass,
        &transferBufferLocation,
        &bufferRegion,
        true);

    SDL_EndGPUCopyPass (copyPass);

    SDL_SubmitGPUCommandBuffer (commandBuffer);
}

void TileMapRenderer::draw (FrameContext* frameContext)
{
    if (frameContext->textureManager->getRawGPUTexture() == nullptr)
    {
        return;
    }

    if (! frameContext->device)
    {
        Logger::log ("Invalid Device", SDL_GetError());
        return;
    }

    if (frameContext->commandBuffer == nullptr)
    {
        Logger::log ("Failed to acquire command buffer", SDL_GetError());
    }

    if (frameContext->swapchainTexture == nullptr)
    {
        Logger::log ("Invalid SwapChain Texture", SDL_GetError());
        return;
    }

    auto colourTargetInfo = SDL_GPUColorTargetInfo {
        .texture = frameContext->swapchainTexture,
        .cycle = false,
        .load_op = SDL_GPU_LOADOP_CLEAR,
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

    auto samplerBinding = SDL_GPUTextureSamplerBinding {
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
        tileData.size() * 6,
        1,
        0,
        0);

    SDL_EndGPURenderPass (renderPass);
}
