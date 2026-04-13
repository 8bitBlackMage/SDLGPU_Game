#pragma once
#include <Graphics/Pipelines/lineRenderer.hpp>
#include <Graphics/Pipelines/spriteBatch.hpp>
#include <Graphics/Structs/line.hpp>
#include <Graphics/Textures/textureManager.hpp>
#include <Graphics/TileMap/tileMapRenderer.hpp>
#include <Graphics/camera.hpp>
#include <Graphics/graphicsContext.hpp>
#include <vector>
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

    void drawEditor();

    void handleEvents (float delta);
    void render();

    void shutdown();

    GraphicsContext graphicsContext;

    Sprite sprite;
    SpriteBatch spriteBatch;

    std::vector<Line> lines;
    LineRenderer lineRenderer;

    TileMapRenderer tileRenderer;

    Camera camera;

    SDL_Window* window;
    SDL_Texture* texture;
    bool running;
    bool showEditor = false;

    float currentFrameTime;
    float lastFrameTime;
};