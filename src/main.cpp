#include <iostream>
#include <SDL3/SDL.h>

int main() {
    std::cout << "Hello world" << std::endl;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    auto * window = SDL_CreateWindow("test",640,480,0);

    bool running = true;
    SDL_Event event;
    while(running) 
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            default: break;
        }
    }
}