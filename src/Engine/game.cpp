#include "LDtkLoader/Project.hpp"
#include <Engine/game.hpp>

#include <Engine/Graphics/graphicsContext.hpp>
#include <Engine/Utils/logger.hpp>
#include <Graphics/Sprites/spriteBatch.hpp>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

Game::Game() : graphicsContext(),
               spriteBatch()
{
    setupSDLContext();
    setupImGuiContext();
    running = true;
}

void Game::run()
{
    while (running)
    {
        handleEvents();
        render();
    }
}

void Game::setupSDLContext()
{
    Logger::getLogger().appendToLog ("Creating Context");
    if (! SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS))
    {
        Logger::getLogger().appendToLog ("SDL CONTEXT Creation failed: ", SDL_GetError());
        exit (-1);
    }

    window = SDL_CreateWindow ("Dungeon Crawler", 1920, 1080, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    if (window == nullptr)
    {
        Logger::log ("SDL WINDOW Creation failed: ", SDL_GetError());
        exit (-1);
    }

    graphicsContext.initContext (window);

    spriteBatch.init (&graphicsContext);

    tileRenderer.init (&graphicsContext);

    graphicsContext.getTextureManager()->beginBatchUpload (&graphicsContext);
    auto tex = graphicsContext.getTextureManager()->loadTexture ("ravioli_atlas.bmp");
    auto tex2 = graphicsContext.getTextureManager()->loadTexture ("PixelPackTOPDOWN8BIT.png");
    graphicsContext.getTextureManager()->endBatchUpload (&graphicsContext);

    sprite = Sprite (tex, 0, 0, 32, 32, 0, 0, 16, 16);

    ldtk::Project project;
    project.loadFromFile ("assets/levels/GdungeonCrawler.ldtk");

    tileRenderer.loadTileMap (project.getWorld().getLevel (0), &graphicsContext);

    SDL_ShowWindow (window);
}

void Game::setupImGuiContext()
{
    graphicsContext.initImGuiGPU();
}

void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent (&event))
    {
        ImGui_ImplSDL3_ProcessEvent (&event);
        switch (event.type)
        {
            case SDL_EVENT_QUIT:
                running = false;
                //#TODO create more sophisticated running shutdown logic
                break;
            default:
                break;
        }
    }
}

void Game::render()
{
    graphicsContext.startFrame();

    tileRenderer.draw (graphicsContext.getFrameContext());

    spriteBatch.draw (&sprite);

    spriteBatch.render (graphicsContext.getFrameContext());

    sceneManager.debugView();
    graphicsContext.debugView();
    spriteBatch.debugView();
    Logger::getLogger().draw();
    graphicsContext.endFrame();
}

void Game::shutdown()
{
    SDL_DestroyWindow (window);
}
