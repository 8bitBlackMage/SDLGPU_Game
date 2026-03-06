#pragma once 
#include <SDL3/SDL.h>
#include "Engine/Graphics/graphicsContext.hpp"
#include "Engine/Graphics/spriteBatch.hpp"

#include "Engine/Scenes/sceneManager.hpp"
/// @brief Class that holds the core Game Data, such as the SDL contexts, scene graph and event handling.
class Game final {
public:
    /// @brief Standard Constructor
    Game();
    /// @brief Function to start and run the game loop.
    void run();

private:
    void setupSDLContext();
    void setupImGuiContext();
    void handleEvents();
    void render();

    void shutdown();

    SceneManager sceneManager;
    GraphicsContext graphicsContext;
    SpriteBatch spriteBatch;

    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Texture * texture;
    bool running;
};