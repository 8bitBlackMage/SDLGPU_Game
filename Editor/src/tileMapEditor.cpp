

#include "tileMapEditor.hpp"
#include "Graphics/graphicsContext.hpp"
#include "LevelData/Maps/scene.hpp"
#include "imgui.h"

void TileMapEditor::init (GraphicsContext* context)
{
    lineRenderer.init (context);
    graphicsContext = context;
    renderTexture.init (context, 1, 1);
    generateGrid (8192, 8292, 32);
}

void TileMapEditor::render (FrameContext* frameContext)
{
    ImGui::Begin ("Map Settings");

    ImGui::InputScalar ("Map Width", ImGuiDataType_U16, &mapWidth);
    ImGui::InputScalar ("Map Height", ImGuiDataType_U16, &mapHeight);
    ImGui::InputScalar ("Grid Size", ImGuiDataType_U16, &gridSize);
    ImGui::Text ("Mouse: X: %i, y: %i", mouseX, mouseY);
    ImGui::End();

    ImGui::Begin ("Map Editor", NULL, ImGuiWindowFlags_HorizontalScrollbar);
    graphicsContext->startRenderTexture (&renderTexture);
    for (auto& line : gridLines)
    {
        lineRenderer.draw (&line);
    }
    lineRenderer.render (frameContext);

    graphicsContext->endRenderTexture();

    //get Mouse Position.

    auto mousePos = ImGui::GetMousePos();
    auto cursorScreenPos = ImGui::GetCursorScreenPos();
    auto scrollPosX = ImGui::GetScrollX();
    auto scrollPosY = ImGui::GetScrollY();

    mouseX = mousePos.x - cursorScreenPos.x - scrollPosX;
    mouseY = mousePos.y - cursorScreenPos.y - scrollPosY;

    if (mapHeight * gridSize != currentLevel.height || mapWidth * gridSize != currentLevel.width || gridSize != currentLevel.gridSize)
    {
        regenerateMap();
    }
    ImGui::Image (renderTexture.getTexture(), { renderTexture.getSize().x, renderTexture.getSize().y });
    ImGui::End();
}

void TileMapEditor::generateGrid (float width, float height, float gridSize)
{
    gridLines.clear();
    for (float y = 0; y < height; y += gridSize)
    {
        gridLines.push_back (Line (0, y, width, y));
    }
    for (float x = 0; x < width; x += gridSize)
    {
        gridLines.push_back (Line (x, 0, x, height));
    }
}

void TileMapEditor::regenerateMap()
{
    if (mapWidth == 0 || mapHeight == 0 || gridSize == 0)
    {
        //no it didn't!
        return;
    }
    renderTexture.resize (graphicsContext, (float) mapWidth * gridSize, (float) mapHeight * gridSize);
    generateGrid ((float) mapWidth * gridSize, (float) mapHeight * gridSize, (float) gridSize);

    currentLevel.width = mapWidth * gridSize;
    currentLevel.height = mapHeight * gridSize;
    currentLevel.gridSize = gridSize;
}