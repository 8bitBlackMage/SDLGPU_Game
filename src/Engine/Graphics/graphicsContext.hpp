#pragma once
#include <SDL3/SDL.h>

#include <filesystem>
#include <map>

/// @brief Wrapper around SDL Renderer Functions to simplify rendering. 
///        With potential of replacing renderer with different underlying framework in future.
class GraphicsContext{
    public:
    GraphicsContext();

    void initContext(SDL_Window* window);

    void initImGuiGPU();

    void debugView();


    void startFrame();


    void endFrame();

    SDL_GPUTexture*  loadTexture(std::string filename);


    SDL_GPUShader * loadShader(std::string filename, 	
                               Uint32 samplerCount, 
                               Uint32 uniformBufferCount,
	                           Uint32 storageBufferCount,
	                           Uint32 storageTextureCount);

    SDL_GPUDevice* getDevice() {return device;}
    SDL_Window*    getWindow() {return window;}
    private:
    SDL_Window * window;
    SDL_GPUDevice * device;
    SDL_GPUSampler * sampler;
    float lastFrameRate;
    bool theme;

    //texture storage pool
    std::map<std::string, SDL_GPUTexture*> textureStorage;
};