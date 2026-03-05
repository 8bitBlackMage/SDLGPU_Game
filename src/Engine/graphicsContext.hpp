#pragma once
#include <SDL3/SDL.h>

/// @brief Wrapper around SDL Renderer Functions to simplify rendering. 
///        With potential of replacing renderer with different underlying framework in future.
class GraphicsContext{
    public:
    GraphicsContext();

    void initContext(SDL_Window* window);

    void initImGuiGPU();

    void debugView();


    void startFrame();


    void endFrame();


    private:
    SDL_Window * window;
    SDL_GPUDevice * device;
};