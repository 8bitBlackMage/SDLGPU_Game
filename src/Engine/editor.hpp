#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <Engine/Graphics/graphicsContext.hpp>
#include <SDL3/SDL.h>
class Editor
{
public:
    Editor() = default;
    void initImGuiContext (GraphicsContext* context);

    void handleEvents (SDL_Event* event);

    void startFrame();
    void drawEditor (GraphicsContext* context);
    void endFrame (FrameContext* context);
};

#endif //EDITOR_HPP