#pragma once
#include <SDL3/SDL.h>

#include <filesystem>
#include <map>

class SpriteBatch;

struct Texture{
    SDL_GPUTexture * texture;
    int width,height;
};

struct FrameContext{
    SDL_GPUDevice * device;
    SDL_GPUCommandBuffer * commandBuffer;
    SDL_GPUTexture* swapchainTexture;
    SDL_GPUViewport viewport;
};

/// @brief Wrapper around SDL Renderer Functions to simplify rendering. 
///        With potential of replacing renderer with different underlying framework in future.
class GraphicsContext{
    public:
    GraphicsContext();

    void initContext(SDL_Window* window);

    void initImGuiGPU();

    void debugView();

    void startFrame();

    FrameContext * getFrameContext() {return &frameContext;}

    void endFrame();

    Texture  loadTexture(std::string filename);

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

    FrameContext frameContext;

    SpriteBatch * testBatch;

    SDL_GPUTexture* renderTexture;

    //texture storage pool
    std::map<std::string, Texture> textureStorage;
};