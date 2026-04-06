#include <Graphics/Textures/texture.hpp>
#include <Graphics/Textures/textureManager.hpp>
#include <Graphics/graphicsContext.hpp>
#include <Utils/logger.hpp>
#include <Utils/vectorTypes.hpp>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <imgui.h>
#include <map>
#include <rectpack2D/finders_interface.h>

TextureManager::TextureManager() : texture (nullptr), uploading (false)
{
}

void TextureManager::beginBatchUpload (GraphicsContext* context)
{
    if (texture != nullptr)
    {
        Logger::log ("Flushing old texture cache");
        SDL_ReleaseGPUTexture (context->getDevice(), texture);
        textures.clear();
        hashedFileNames.clear();
    }
    uploading = true;
}

Texture TextureManager::loadTexture (std::string filename)
{
    assert (uploading == true);
    std::hash<std::string> hash;
    size_t id = hash (filename);
    if (! hashedFileNames.contains (id))
    {
        Logger::log ("Appending:", filename, "to texture list");
        textures.push_back (TextureData (filename, id));
        hashedFileNames[id] = textures.size() - 1;
    }
    Logger::log ("Texture:", filename, "at ID:", id);
    return Texture (id, this);
}

void TextureManager::endBatchUpload (GraphicsContext* context)
{
    auto timeAtStart = SDL_GetTicks();
    Logger::log ("Regenerating Texture Atlas");
    std::map<TextureData*, SDL_Surface*> surfaceBuffer;

    for (auto& textureData : textures)
    {
        std::filesystem::path imgpath = "assets/images/" + textureData.fileName;

        auto surface = IMG_Load (imgpath.c_str());

        if (surface == nullptr)
        {
            Logger::log ("failed to load", textureData.fileName, "from disk");
        }
        textureData.get_rect() = { 0, 0, surface->w, surface->h, false };
        surfaceBuffer[&textureData] = surface;
    }

    textureSize = solve();

    SDL_GPUTextureCreateInfo texCreateInfo = {};
    texCreateInfo.type = SDL_GPU_TEXTURETYPE_2D;
    texCreateInfo.format = SDL_GPU_TEXTUREFORMAT_B8G8R8A8_UNORM;
    texCreateInfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
    texCreateInfo.width = textureSize.x;
    texCreateInfo.height = textureSize.y;
    texCreateInfo.layer_count_or_depth = 1;
    texCreateInfo.num_levels = 1;
    texCreateInfo.sample_count = SDL_GPU_SAMPLECOUNT_1;

    texture = SDL_CreateGPUTexture (context->getDevice(), &texCreateInfo);

    SDL_GPUTransferBufferCreateInfo transferBufferCreateInfo = {};
    transferBufferCreateInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    transferBufferCreateInfo.size = textureSize.x * textureSize.y * 4;
    SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer (context->getDevice(), &transferBufferCreateInfo);

    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer (context->getDevice());
    for (auto& surfaceData : surfaceBuffer)
    {
        auto textureData = surfaceData.first;
        auto surface = surfaceData.second;

        uint32_t uploadPitch = surface->pitch;
        void* texturePointer = SDL_MapGPUTransferBuffer (context->getDevice(), transferBuffer, true);

        for (int y = 0; y < surface->h; y++)
        {
            memcpy ((void*) ((uintptr_t) texturePointer + y * uploadPitch), (char*) surface->pixels + y * uploadPitch, uploadPitch);
        }

        SDL_UnmapGPUTransferBuffer (context->getDevice(), transferBuffer);

        SDL_GPUTextureTransferInfo transferInfo = {};
        transferInfo.offset = 0;
        transferInfo.transfer_buffer = transferBuffer;

        SDL_GPUTextureRegion textureRegion = {};
        textureRegion.texture = texture;
        textureRegion.x = (Uint32) textureData->get_rect().x;
        textureRegion.y = (Uint32) textureData->get_rect().y;
        textureRegion.w = (Uint32) textureData->get_rect().w;
        textureRegion.h = (Uint32) textureData->get_rect().h;
        textureRegion.d = 1;

        SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass (cmd);
        SDL_UploadToGPUTexture (copy_pass, &transferInfo, &textureRegion, false);
        SDL_EndGPUCopyPass (copy_pass);
    }
    SDL_ReleaseGPUTransferBuffer (context->getDevice(), transferBuffer);
    if (! SDL_SubmitGPUCommandBuffer (cmd))
    {
        Logger::log (SDL_GetError());
    }

    auto timeAtEnd = SDL_GetTicks();
    auto timeTaken = timeAtEnd - timeAtStart;

    Logger::log ("Generation complete, took:", timeTaken, "ms");
    uploading = false;
}

Texture TextureManager::getTextureForFile (std::string filename)
{
    std::hash<std::string> hash;
    size_t id = hash (filename);
    if (hashedFileNames.contains (id))
    {
        return Texture { id, this };
    }
    assert (false);
    return Texture {};
}

void TextureManager::debugView()
{
    ImGui::Begin ("Textures");
    for (auto& t : textures)
    {
        ImGui::Text ("%s", t.fileName.c_str());
        ImGui::SameLine();

        auto uv = pixelToUV (t.get_rect().x,
                             t.get_rect().y,
                             t.get_rect().w,
                             t.get_rect().h);
        ImGui::Text ("Width: %ipx, Height %ipx", t.get_rect().w, t.get_rect().h);
        ImGui::Text ("ID: %u", (int) t.id);
        ImGui::Image ((ImTextureID) (intptr_t) texture,
                      { 200, 200 },
                      { uv.x, uv.y },
                      { uv.w, uv.h });
    }
    ImGui::End();
    ImGui::Begin ("Atlas View");
    ImGui::Text ("zoom");
    ImGui::SameLine();
    if (ImGui::Button ("+"))
    {
        zoomFactor++;
    }
    ImGui::SameLine();
    if (ImGui::Button ("-"))
    {
        zoomFactor--;
    }
    ImGui::BeginChild ("Scrollable");
    ImGui::Image ((ImTextureID) (intptr_t) texture, { static_cast<float> (textureSize.x * zoomFactor), static_cast<float> (textureSize.y * zoomFactor) });
    ImGui::EndChild();
    ImGui::End();
}

Vec2<int> TextureManager::solve()
{
    constexpr bool allow_flip = true;
    const auto runtime_flipping_mode = rectpack2D::flipping_option::ENABLED;

    using spaces_type = rectpack2D::empty_spaces<allow_flip, rectpack2D::default_empty_spaces>;

    using rect_type = rectpack2D::output_rect_t<spaces_type>;

    const auto max_side = 1000;

    const auto discard_step = -4;

    auto report_successful = [] (rect_type&)
    {
        return rectpack2D::callback_result::CONTINUE_PACKING;
    };

    auto report_unsuccessful = [] (rect_type&)
    {
        return rectpack2D::callback_result::ABORT_PACKING;
    };

    const auto result_size = rectpack2D::find_best_packing<spaces_type> (
        textures,
        rectpack2D::make_finder_input (
            max_side,
            discard_step,
            report_successful,
            report_unsuccessful,
            runtime_flipping_mode));

    return Vec2 (result_size.w, result_size.h);
}

Vec4<float> TextureManager::pixelToUV (int x, int y, int w, int h)
{
    return Vec4<float> {
        (float) x / textureSize.x,
        (float) y / textureSize.y,
        (float) (x + w) / textureSize.x,
        (float) (y + h) / textureSize.y,
    };
}
