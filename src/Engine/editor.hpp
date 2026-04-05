#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <Engine/Graphics/graphicsContext.hpp>
#include <SDL3/SDL.h>
class Editor
{
public:
    Editor() = default;
    void initImGuiContext (GraphicsContext* context);
    void startFrame();
    void drawEditor();
    void endFrame (FrameContext context);
};

#endif //EDITOR_HPP