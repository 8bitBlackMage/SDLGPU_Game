#pragma once
#include <Engine/Graphics/textureManager.hpp>
#include <SDL3/SDL.h>
#include <map>

/// @brief Struct containing per frame data, used for rendering.
struct FrameContext
{
    SDL_GPUDevice* device;
    SDL_GPUCommandBuffer* commandBuffer;
    SDL_GPUTexture* swapchainTexture;
    SDL_GPUViewport viewport;
};

/// @brief Wrapper around SDL Renderer Functions to simplify rendering.
///        With potential of replacing renderer with different underlying framework in future.
class GraphicsContext final
{
public:
    GraphicsContext();

    ~GraphicsContext();

    /// @brief Sets up SDL GPU device state, must be called first otherwise no other functions will work.
    /// @param window pointer to SDL Window
    void initContext (SDL_Window* window);

    void shutdown();

    /// @brief Sets up ImGUI context.
    void initImGuiGPU();

    /// @brief Renders ImGUi Windows.
    void debugView();

    /// @brief Sets up the render state for a given frame. must be called before other render calls.
    void startFrame();

    /// @brief Gets the Frame Context struct for a frame.
    /// @return Struct containing this frames data.
    FrameContext* getFrameContext() { return &frameContext; }

    /// @brief submits render passes to the GPU.
    void endFrame();

    /// @brief Loads shader into GPU memory to be bound to render pipeline.
    /// @param filename Filename of shader.
    /// @param samplerCount Number of Texture Samplers used by shader
    /// @param uniformBufferCount Number of Uniform Buffers used by shader.
    /// @param storageBufferCount Number of Storage Buffers used by shader.
    /// @param storageTextureCount Number of Textures used by shader.
    /// @return GPU Shader pointer.
    SDL_GPUShader* loadShader (std::string filename,
                               Uint32 samplerCount,
                               Uint32 uniformBufferCount,
                               Uint32 storageBufferCount,
                               Uint32 storageTextureCount);

    /// @brief Getter for SDL Device.
    /// @return pointer to device.
    SDL_GPUDevice* getDevice() { return device; }

    /// @brief Getter for SDL Window.
    /// @return pointer to window.
    SDL_Window* getWindow() { return window; }

private:
    SDL_Window* window;
    SDL_GPUDevice* device;
    SDL_GPUSampler* sampler;

    FrameContext frameContext;
    SDL_GPUTexture* renderTexture;

    //texture storage pool
    std::map<std::string, Texture> textureStorage;
};