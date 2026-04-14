#pragma once

#include "Graphics/graphicsContext.hpp"
#include "SDL3/SDL_pixels.h"
class BackgroundRenderer
{
public:
    BackgroundRenderer() = default;

    /// @brief Initialises the Sprite Batcher. must be called after initialising the Graphics context.
    /// @param context Pointer to the Graphics Context.
    void init (GraphicsContext* context);

    /// @brief Renders the SpriteBatch data to the screen, should only be called once per frame.
    /// @param context Pointer to the Frame Context.
    void render (FrameContext* frameContext);

    void setBackgroundColour (SDL_Color colour);

private:
    SDL_FColor backgroundColour;
    SDL_GPUGraphicsPipeline* RenderPipeline;
};