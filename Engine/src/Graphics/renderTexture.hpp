#pragma once

#include "Graphics/graphicsContext.hpp"
#include "glm/ext/vector_float2.hpp"

#include <SDL3/SDL.h>
#include <glm/vec2.hpp>
class RenderTexture final
{
public:
    RenderTexture() = default;
    RenderTexture (GraphicsContext* context, float width, float height);
    void resize (GraphicsContext* context, float width, float height);
    void init (GraphicsContext* context, float width, float height);

    SDL_GPUTexture* getTexture();
    glm::vec2 getSize();

private:
    SDL_GPUTexture* target;
    glm::vec2 targetSize;
};