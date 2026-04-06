#pragma once

#include <editor.hpp>

#include <Graphics/Sprites/spriteBatch.hpp>
#include <Graphics/Textures/textureManager.hpp>
#include <Graphics/TileMap/tileMapRenderer.hpp>
#include <Graphics/camera.hpp>
#include <Graphics/graphicsContext.hpp>
/// @brief Class that holds the core Game Data, such as the SDL contexts, scene graph and event handling.
class Game final
{
public:
    /// @brief Standard Constructor
    Game();
    /// @brief Function to start and run the game loop.
    void run();

private:
    void loadAssets();

    void handleEvents (float delta);
    void render();

    void shutdown();

    Editor editor;

    GraphicsContext graphicsContext;

    Sprite sprite;
    SpriteBatch spriteBatch;

    TileMapRenderer tileRenderer;

    Camera camera;

    SDL_Window* window;
    SDL_Texture* texture;
    bool running;
    bool showEditor = false;

    float currentFrameTime;
    float lastFrameTime;
};