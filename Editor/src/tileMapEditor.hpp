#pragma once

#include "Graphics/Pipelines/backgroundRenderer.hpp"
#include "Graphics/Pipelines/lineRenderer.hpp"
#include "Graphics/Pipelines/spriteBatch.hpp"
#include "Graphics/Structs/line.hpp"
#include "Graphics/graphicsContext.hpp"
#include "Graphics/renderTexture.hpp"

#include "LevelData/Maps/scene.hpp"

class TileMapEditor
{
public:
    TileMapEditor() = default;

    void init (GraphicsContext* context);

    void render (FrameContext* context);

private:
    void regenerateMap();

    void generateGrid (float width, float height, float gridSize);
    std::vector<Line> gridLines;
    RenderTexture renderTexture;
    //Renderers
    BackgroundRenderer background;
    LineRenderer lineRenderer;
    SpriteBatch SpriteRenderer;
    GraphicsContext* graphicsContext;

    int mapWidth = 0;
    int mapHeight = 0;
    int gridSize = 0;

    int mouseX;
    int mouseY;

    Texture testTex;

    Scene currentLevel;
};
