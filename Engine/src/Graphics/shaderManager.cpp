#include "Graphics/graphicsContext.hpp"
#include "SDL3_shadercross/SDL_shadercross.h"
#include "Utils/logger.hpp"
#include <Graphics/shaderManager.hpp>
#include <filesystem>
#include <fstream>
#include <iterator>
void ShaderManager::init (GraphicsContext* context)
{
    graphicsContext = context;
    SDL_ShaderCross_Init();
}

SDL_GPUShader* ShaderManager::loadShader (std::filesystem::path filepath)
{
    if (shaderCache.contains (filepath))
    {
        return shaderCache.at (filepath);
    }
    // Handle shader loading and compilation here...

    return nullptr;
}

SDL_GPUShader* ShaderManager::compileShader (std::filesystem::path filepath)
{
    if (! graphicsContext)
    {
        Logger::log ("Graphics Context invalid");
        return nullptr;
    }

    //1. load HLSL file from disk

    if (! std::filesystem::is_regular_file (filepath))
    {
        Logger::log ("Failed to Load", filepath, "not found on disk.");
        return nullptr;
    }

    std::ifstream hlslFile (filepath.string());
    std::string hlslSource { std::istreambuf_iterator (hlslFile), std::istreambuf_iterator<char>() };

    if (hlslSource.length() == 0)
    {
        Logger::log ("Failed to Load", filepath, "unable to read");
        return nullptr;
    }
    //2. compile HLSL to SPIRV

    auto hlslShaderInfo = SDL_ShaderCross_HLSL_Info {
        .source = hlslSource.c_str(),
        .entrypoint = "main",
        .shader_stage = SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT
    };
    size_t spirvSize;
    auto* spirvShader = SDL_ShaderCross_CompileSPIRVFromHLSL (&hlslShaderInfo, &spirvSize);
    if (spirvShader == nullptr)
    {
        Logger::log ("Failed to Compile Shader: ", SDL_GetError());
        return nullptr;
    }
    //3. Translate to native Shader Format.

    auto shaderInfo = SDL_ShaderCross_SPIRV_Info {
        .bytecode = (Uint8*) spirvShader,
        .bytecode_size = spirvSize,
        .entrypoint = "main",
        .shader_stage = SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT,
    };

    auto* shader = SDL_ShaderCross_CompileGraphicsShaderFromSPIRV (graphicsContext->getDevice(), &shaderInfo, nullptr, 0);

    if (shader == nullptr)
    {
        Logger::log ("Failed to convert SPIRV to shader", SDL_GetError());
        return nullptr;
    }

    //4. cache resulting data for future loading.
    shaderCache[filepath] = shader;

    return shader;
}
