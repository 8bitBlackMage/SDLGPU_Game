

#include "sceneEditor.hpp"
#include "Graphics/Structs/sprite.hpp"
#include "Graphics/graphicsContext.hpp"
#include "LevelData/Maps/scene.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "windowIDs.hpp"

void SceneEditor::init (Scene* scene, GraphicsContext* context)
{
    currentLevel = scene;
    graphicsContext = context;
    if (scene != nullptr)
    {
        // mapHeight = currentLevel->height / currentLevel->gridSize;
        // mapWidth = currentLevel->width / currentLevel->gridSize;
        // gridSize = currentLevel->gridSize;
    }
    background.init (graphicsContext);
    lineRenderer.init (graphicsContext);
    SpriteRenderer.init (graphicsContext);
    renderTexture.init (graphicsContext, 1, 1);

    regenerateMap();

    graphicsContext->getTextureManager()->beginBatchUpload (graphicsContext);
    testTex = graphicsContext->getTextureManager()->loadTexture ("player.png");
    graphicsContext->getTextureManager()->endBatchUpload (graphicsContext);
}

void SceneEditor::setupDockSpace (ImGuiID centre, ImGuiID left, ImGuiID right, ImGuiID bottom)
{
    ImGui::DockBuilderDockWindow (WindowIDs::sceneSettings, left);
    ImGui::DockBuilderDockWindow (WindowIDs::sceneEditor, centre);
}

void SceneEditor::present (FrameContext* frameContext)
{
    if (currentLevel == nullptr)
    {
        return;
    }

    ImGui::Begin (WindowIDs::sceneSettings);
    ImGui::InputScalar ("Map Width", ImGuiDataType_U16, &mapWidth);
    ImGui::InputScalar ("Map Height", ImGuiDataType_U16, &mapHeight);
    ImGui::InputScalar ("Grid Size", ImGuiDataType_U16, &gridSize);
    ImGui::Text ("Mouse: X: %i, y: %i", mouseX, mouseY);
    ImGui::Text ("grid: X: %i, y: %i", (int) floor (mouseX / gridSize), (int) floor (mouseY / gridSize));
    ImGui::End();

    ImGui::SetNextWindowSizeConstraints ({ 800, 600 }, { 1920, 1080 });
    ImGui::Begin (WindowIDs::sceneEditor, NULL, ImGuiWindowFlags_HorizontalScrollbar);
    //get Mouse Position.

    auto mousePos = ImGui::GetMousePos();
    auto cursorScreenPos = ImGui::GetCursorScreenPos();
    auto scrollPosX = ImGui::GetScrollX();
    auto scrollPosY = ImGui::GetScrollY();

    mouseX = mousePos.x - cursorScreenPos.x;
    mouseY = mousePos.y - cursorScreenPos.y;

    graphicsContext->startRenderTexture (&renderTexture);
    for (auto& line : gridLines)
    {
        lineRenderer.draw (&line);
    }
    background.render (frameContext);

    auto previewPosX = floor ((float) mouseX / gridSize) * gridSize;
    auto previewPosY = floor ((float) mouseY / gridSize) * gridSize;

    auto sprite = Sprite { testTex, static_cast<int> (previewPosX), static_cast<int> (previewPosY), gridSize, gridSize, 0, 0, 16, 16 };
    SpriteRenderer.draw (&sprite);

    lineRenderer.render (frameContext);
    SpriteRenderer.render (frameContext);
    graphicsContext->endRenderTexture();
    if (currentLevel != nullptr)
    {
        // if (mapHeight * gridSize != currentLevel->height || mapWidth * gridSize != currentLevel->width || gridSize != currentLevel->gridSize)
        // {
        //     regenerateMap();
        // }
    }
    ImGui::Image (renderTexture.getTexture(), { renderTexture.getSize().x, renderTexture.getSize().y });
    ImGui::End();
}

void SceneEditor::generateGrid (float width, float height, float gridSize)
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

void SceneEditor::regenerateMap()
{
    if (mapWidth == 0 || mapHeight == 0 || gridSize == 0)
    {
        //no it didn't!
        return;
    }
    renderTexture.resize (graphicsContext, (float) mapWidth * gridSize, (float) mapHeight * gridSize);
    generateGrid ((float) mapWidth * gridSize, (float) mapHeight * gridSize, (float) gridSize);

    // currentLevel->width = mapWidth * gridSize;
    // currentLevel->height = mapHeight * gridSize;
    // currentLevel->gridSize = gridSize;
}