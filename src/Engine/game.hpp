#pragma once
#include "Engine/Graphics/TileMap/tileMapRenderer.hpp"
#include <Engine/Graphics/Textures/textureManager.hpp>
#include <Engine/Graphics/graphicsContext.hpp>
#include <Graphics/Sprites/spriteBatch.hpp>
#include <Graphics/camera.hpp>
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
    void loadAssets();
    void setupImGuiContext();
    void handleEvents (float delta);
    void render();

    void shutdown();

    GraphicsContext graphicsContext;

    Sprite sprite;
    SpriteBatch spriteBatch;

    TileMapRenderer tileRenderer;

    Camera camera;

    SDL_Window* window;
    SDL_Texture* texture;
    bool running;

    float currentFrameTime;
    float lastFrameTime;
};