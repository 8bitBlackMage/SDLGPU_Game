#include "game.hpp"

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include <iostream>

Game::Game() {
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
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS))
    {
        std::cerr << "SDL CONTEXT Creation failed: "  << SDL_GetError() << std::endl;
        exit(-1);
    }


    window = SDL_CreateWindow("TEST", 1920,1080, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    if (window == nullptr)
    {
        std::cerr << "SDL WINDOW Creation failed: " << SDL_GetError() << std::endl;
        exit(-1);
    }
    renderer = SDL_CreateRenderer(window,nullptr);
    if (renderer == nullptr){
        std::cerr << "SDL RENDERER Creation failed: " << SDL_GetError() << std::endl;
        exit(-1);
    }

    if (!SDL_SetRenderVSync(renderer, 1))
    {
        std::cerr << "SDL VSync set failed: "  << SDL_GetError() << std::endl;
        exit(-1);
    }

    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_TARGET,
                                320,240);

    SDL_ShowWindow(window);
}

void Game::setupImGuiContext()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplSDL3_InitForSDLRenderer(window,renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
    ImGui::StyleColorsLight();
    ImGuiStyle& style = ImGui::GetStyle();

    float mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    style.ScaleAllSizes(mainScale);
    style.FontScaleDpi = mainScale;
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
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("debug");
        {
            ImGuiIO& io = ImGui::GetIO();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        }
        ImGui::End();
        ImGui::Begin("Game Render");
        {
            SDL_SetRenderTarget(renderer,texture);
            SDL_RenderClear(renderer);
            SDL_FRect Rect{0.0,0.0,32.0,32.0};
            SDL_SetRenderDrawColor(renderer,255,255,255,255);
            SDL_RenderRect(renderer,&Rect);

        SDL_SetRenderTarget(renderer,nullptr);
        auto winSize = ImGui::GetContentRegionAvail();
        float widthScale = winSize.x / 320.;
        float heightScale = winSize.y / 240;
        float scale = int (std::min(widthScale,heightScale));
        


        ImGui::Image(texture,{320 * scale, 240 *scale});
        }
        ImGui::End();


        sceneManager.debugView();

        ImGui::Render();
        ImGuiIO io = ImGui::GetIO();
        SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
}
