#pragma once

#include <Graphics/graphicsContext.hpp>
#include <queue>

class Line;

class LineRenderer
{
public:
    LineRenderer() = default;

    /// @brief Initialises the Sprite Batcher. must be called after initialising the Graphics context.
    /// @param context Pointer to the Graphics Context.
    void init (GraphicsContext* context);

    void draw (Line* line);

    /// @brief Renders the SpriteBatch data to the screen, should only be called once per frame.
    /// @param context Pointer to the Frame Context.
    void render (FrameContext* context);
    friend class Line;

    std::queue<Line*> lineQueue;

private:
    SDL_GPUGraphicsPipeline* RenderPipeline;
    SDL_GPUSampler* sampler;

    SDL_GPUTransferBuffer* transferBuffer;
    SDL_GPUBuffer* dataBuffer;
};