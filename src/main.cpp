#include <iostream>
#include <SDL3/SDL.h>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

int main() {
    std::cout << "Hello world" << std::endl;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    auto * window = SDL_CreateWindow("test",640,480,SDL_WINDOW_RESIZABLE);
    auto * renderer = SDL_CreateRenderer(window,nullptr);


    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplSDL3_InitForSDLRenderer(window,renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    bool running = true;
    SDL_Event event;
    while(running) 
    {
        SDL_PollEvent(&event);
        ImGui_ImplSDL3_ProcessEvent(&event);
        switch(event.type)
        {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            default: break;
        }

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("TEST");
        ImGui::End();


        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);


    }
}