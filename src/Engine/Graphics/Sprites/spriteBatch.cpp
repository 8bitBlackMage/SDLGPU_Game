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

    //texture = context->loadTexture ("ravioli_atlas.bmp");
}

void SpriteBatch::draw (Sprite* sprite)
{
    if (sprite != nullptr)
    {
        spriteQueue.push (sprite);
    }
}

static float uCoords[4] = { 0.0f, 0.5f, 0.0f, 0.5f };
static float vCoords[4] = { 0.0f, 0.0f, 0.5f, 0.5f };

void SpriteBatch::render (FrameContext* frameContext)
{
    float timeAtStart = SDL_GetTicksNS();
    Matrix4x4 cameraMatrix = Matrix4x4::CreateOrthographicOffCenter (
        0,
        640,
        480,
        0,
        0,
        -1);

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

    // for (uint32_t y = 0; y < 480 / 32; y++)
    // {
    //     for (uint32_t x = 0; x < 640 / 32; x++)
    //     {
    //         int i = (x * 640 / 32) + y;
    //         Sint32 ravioli = SDL_rand (4);
    //         data[i].x = x * 32;
    //         data[i].y = y * 32;
    //         data[i].z = 0;
    //         data[i].rotation = SDL_randf() * SDL_PI_F * 2;
    //         data[i].w = 32;
    //         data[i].h = 32;
    //         data[i].textureU = uCoords[1];
    //         data[i].textureV = vCoords[1];
    //         data[i].textureW = 0.5f;
    //         data[i].textureH = 0.5f;
    //         data[i].r = 1.0f;
    //         data[i].g = 1.0f;
    //         data[i].b = 1.0f;
    //         data[i].a = 1.0f;
    //     }
    // }
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

    auto samplerBinding = (SDL_GPUTextureSamplerBinding) {
        .texture = texture.texture,
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
        &cameraMatrix,
        sizeof (Matrix4x4));
    SDL_DrawGPUPrimitives (
        renderPass,
        spritesToDraw * 6,
        1,
        0,
        0);

    SDL_EndGPURenderPass (renderPass);
    float timeAtEnd = SDL_GetTicksNS();

    // renderTime[renderTimeIndex] = (timeAtEnd - timeAtStart);
    // renderTimeIndex++;
    // if (renderTimeIndex >= 500)
    // {
    //     renderTimeIndex = 0;
    // }
}

void SpriteBatch::debugView()
{
    ImGui::Begin ("SpriteBatch");
    {
        ImGui::SliderInt ("Sprites", &spritesToDraw, 0, 428);
        ImGui::End();
    }
}
