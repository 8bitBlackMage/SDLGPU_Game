#pragma once

#include <Graphics/graphicsContext.hpp>

class ImDrawList;
class ImDrawCmd;
void ImDrawCallback_ImplSDLGPU3_SetSampler (const ImDrawList* /*parent_list*/, const ImDrawCmd* cmd);

namespace ImGUIHelpers
{
void initContext (GraphicsContext* context);

void handleEvents (SDL_Event* event);

void startFrame();

void endFrame (FrameContext* frameContext);
} // namespace ImGUIHelpers
