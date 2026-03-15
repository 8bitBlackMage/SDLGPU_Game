#pragma once
#include "../graphicsContext.hpp"
#include <Engine/Graphics/Sprites/sprite.hpp>
#include <queue>

class Sprite;

class SpriteBatch
{
public:
    SpriteBatch();

    /// @brief Initialises the Sprite Batcher. must be called after initialising the Graphics context.
    /// @param context Pointer to the Graphics Context.
    void init (GraphicsContext* context);

    void draw (Sprite* sprite);

    /// @brief Renders the SpriteBatch data to the screen, should only be called once per frame.
    /// @param context Pointer to the Frame Context.
    void render (FrameContext* context);

    void debugView();

    friend class Sprite;

private:
    std::queue<Sprite*> spriteQueue;

    SDL_GPUGraphicsPipeline* RenderPipeline;
    SDL_GPUSampler* sampler;

    SDL_GPUTransferBuffer* transferBuffer;
    SDL_GPUBuffer* dataBuffer;

    int spritesToDraw = 256;
    float renderTime[500];
    int renderTimeIndex;
};