#pragma once
#include <Engine/Graphics/graphicsContext.hpp>
#include <Engine/Graphics/textureManager.hpp>
#include <Engine/Scenes/sceneManager.hpp>
#include <Graphics/Sprites/spriteBatch.hpp>
#include <SDL3/SDL.h>
/// @brief Class that holds the core Game Data, such as the SDL contexts, scene graph and event handling.
class Game final
{
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
    TextureManager textureManager;

    Sprite sprite;
    SpriteBatch spriteBatch;

    SDL_Window* window;
    SDL_Texture* texture;
    bool running;
};