#include "SDL3_shadercross/SDL_shadercross.h"
#include "Utils/logger.hpp"
#include <Graphics/graphicsContext.hpp>
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
    Logger::log ("Loading:", filepath);
    Logger::log ("Attempting to find Shader in cache");
    if (shaderCache.contains (filepath))
    {
        return shaderCache.at (filepath);
    }
    // Handle shader loading and compilation here...

    return compileShader (filepath);
}

SDL_GPUShader* ShaderManager::compileShader (std::filesystem::path filepath)
{
    Logger::log ("Compiling Shader from HLSL");
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

    // figure out what type of shader this is.
    SDL_ShaderCross_ShaderStage stage;
    if (filepath.string().contains ("frag"))

    {
        stage = SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT;
    }
    else if (filepath.string().contains ("vert"))
    {
        stage = SDL_SHADERCROSS_SHADERSTAGE_VERTEX;
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
        .shader_stage = stage,
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
        .shader_stage = stage,
        .props = 0,
    };
    Logger::log ("Compiled Shader to SPIRV");

    auto reflectInfo = SDL_ShaderCross_ReflectGraphicsSPIRV (
        (Uint8*) spirvShader, spirvSize, 0);

    Logger::log ("Generating native shader resource");

    auto* shader = SDL_ShaderCross_CompileGraphicsShaderFromSPIRV (graphicsContext->getDevice(), &shaderInfo, &reflectInfo->resource_info, 0);

    if (shader == nullptr)
    {
        Logger::log ("Failed to convert SPIRV to shader", SDL_GetError());
        return nullptr;
    }

    //4. cache resulting data for future loading.
    shaderCache[filepath] = shader;

    return shader;
}
