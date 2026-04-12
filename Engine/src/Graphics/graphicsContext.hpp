#pragma once
#include "SDL3/SDL_gpu.h"
#include "glm/ext/matrix_float4x4.hpp"
#include <Graphics/Textures/textureManager.hpp>
#include <SDL3/SDL.h>
#include <Utils/vectorTypes.hpp>
#include <glm/mat4x4.hpp>

/// @brief Struct containing per frame data, used for rendering.
struct FrameContext
{
    SDL_Window* window;
    SDL_GPUDevice* device;
    SDL_GPUCommandBuffer* commandBuffer;
    SDL_GPUTexture* swapchainTexture;
    glm::mat4x4 cameraMatrix;
    SDL_GPUViewport viewport;
    TextureManager* textureManager;
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
    void initContext();

    ///@brief Sets the size of the Render Texture.
    ///@param w width of the texture in pixels.
    ///@param h height of the texture in pixels.
    ///@returns bool if texture was created successfully.
    bool setRenderTextureSize (float w, float h);

    void shutdown();

    /// @brief Sets up the render state for a given frame. must be called before other render calls.
    void startFrame();

    /// @brief Gets the Frame Context struct for a frame.
    /// @return Struct containing this frames data.
    FrameContext* getFrameContext() { return &frameContext; }

    void startRenderTexture();

    void endRenderTexture();
    void blitRenderTexture();
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

    SDL_GPUTexture* getRenderTexture() { return renderTexture; }

    inline TextureManager* getTextureManager() { return &textureManager; }

private:
    SDL_Window* window;
    SDL_GPUDevice* device;
    SDL_GPUSampler* sampler;

    FrameContext frameContext;

    Vec2<float> renderTextureSize;
    SDL_GPUTexture* renderTexture;

    SDL_GPUTexture* swapChainTexture;
    TextureManager textureManager;
};
