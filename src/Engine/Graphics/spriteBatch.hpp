#pragma once
#include "graphicsContext.hpp"

struct SpriteData
{
    float x, y, z;
    float rotation;
    float w, h;
    float paddingA, paddingB;
    float textureU, textureV, textureW, textureH;
    float r, g, b, a;
};

class SpriteBatch
{
public:
    SpriteBatch();

    /// @brief Initialises the Sprite Batcher. must be called after initialising the Graphics context.
    /// @param context Pointer to the Graphics Context.
    void init (GraphicsContext* context);

    /// @brief Renders the SpriteBatch data to the screen, should only be called once per frame.
    /// @param context Pointer to the Frame Context.
    void render (FrameContext* context);

    void drawDebugInfo();
private:
    SDL_GPUGraphicsPipeline* RenderPipeline;
    SDL_GPUSampler* sampler;
    Texture texture;

    SDL_GPUTransferBuffer* transferBuffer;
    SDL_GPUBuffer* dataBuffer;

    GraphicsContext* context;

    int spritesToDraw = 256;
    float renderTime;
};