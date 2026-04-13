#pragma once

#include "Graphics/Pipelines/lineRenderer.hpp"
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
    LineRenderer lineRenderer;
    GraphicsContext* graphicsContext;

    int mapWidth;
    int mapHeight;
    int gridSize;

    int mouseX;
    int mouseY;

    Scene currentLevel;
};
