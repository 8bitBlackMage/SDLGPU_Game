#pragma once

#include "Graphics/graphicsContext.hpp"
#include "SDL3/SDL_gpu.h"
#include <filesystem>
class ShaderManager
{
public:
    ShaderManager();
    void init (GraphicsContext* context);

    SDL_GPUShader* loadShader (std::filesystem::path path);

private:
    SDL_GPUShader* compileShader (std::filesystem::path path);
    //store compiled shaders in memory in case reloading is needed.
    std::map<std::filesystem::path, SDL_GPUShader*> shaderCache;
    GraphicsContext* graphicsContext;
};
