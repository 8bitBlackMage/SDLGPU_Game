#include "game.hpp"

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include <iostream>
#include "Graphics/spriteBatch.hpp"

#include "logger.hpp"

Game::Game():
    graphicsContext()
{
    setupSDLContext();
    setupImGuiContext();
    running = true;
}

void Game::run()
{
    while(running)
    {
        handleEvents();

        render();
    }
}

void Game::setupSDLContext()
{
    Logger::getLogger().appendToLog("Creating Context");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS))
    {
        Logger::getLogger().appendToLog("SDL CONTEXT Creation failed: " , SDL_GetError());
        exit(-1);
    }


    window = SDL_CreateWindow("Dungeon Crawler", 1920,1080, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    if (window == nullptr)
    {
        Logger::log( "SDL WINDOW Creation failed: " , SDL_GetError() );
        exit(-1);
    }
    renderer = SDL_CreateRenderer(window,nullptr);
    if (renderer == nullptr){
        Logger::log(  "SDL RENDERER Creation failed: " , SDL_GetError());
        exit(-1);
    }

    if (!SDL_SetRenderVSync(renderer, 1))
    {
        Logger::log("SDL VSync set failed: ", SDL_GetError());
        exit(-1);
    }

    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_TARGET,
                                320,240);

    graphicsContext.initContext(window);

    SpriteBatch test(&graphicsContext);

    SDL_ShowWindow(window);
}

void Game::setupImGuiContext()
{
    graphicsContext.initImGuiGPU();

    // ImGui_ImplSDL3_InitForSDLRenderer(window,renderer);
    // ImGui_ImplSDLRenderer3_Init(renderer);

}

void Game::handleEvents()
{
    SDL_Event event;
        while(SDL_PollEvent(&event)){
        ImGui_ImplSDL3_ProcessEvent(&event);
        switch(event.type)
        {
            case SDL_EVENT_QUIT:
                running = false;
                //#TODO create more sophisticated running shutdown logic
                break;
            default: break;
        }
    }
}

void Game::render()
{
    graphicsContext.startFrame();


    sceneManager.debugView();
    graphicsContext.debugView();
    Logger::getLogger().draw();
    graphicsContext.endFrame();
}

void Game::shutdown()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
