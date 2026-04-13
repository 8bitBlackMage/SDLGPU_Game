#pragma once
#include <Graphics/Structs/sprite.hpp>
#include <Graphics/graphicsContext.hpp>
#include <queue>

class Sprite;

class SpriteBatch
{
public:
    SpriteBatch() = default;

    /// @brief Initialises the Sprite Batcher. must be called after initialising the Graphics context.
    /// @param context Pointer to the Graphics Context.
    void init (GraphicsContext* context);

    void draw (Sprite* sprite);

    /// @brief Renders the SpriteBatch data to the screen, should only be called once per frame.
    /// @param context Pointer to the Frame Context.
    void render (FrameContext* context);
    friend class Sprite;

private:
    std::queue<Sprite*> spriteQueue;

    SDL_GPUGraphicsPipeline* RenderPipeline;
    SDL_GPUSampler* sampler;

    SDL_GPUTransferBuffer* transferBuffer;
    SDL_GPUBuffer* dataBuffer;
    float renderTime[500];
    int renderTimeIndex;
};