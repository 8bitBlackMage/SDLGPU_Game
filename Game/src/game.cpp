

#include <LDtkLoader/Project.hpp>

#include <game.hpp>

#include <Graphics/Sprites/spriteBatch.hpp>
#include <Graphics/camera.hpp>
#include <Graphics/graphicsContext.hpp>
#include <Imgui/imguiHelpers.hpp>
#include <Utils/logger.hpp>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

Game::Game() : graphicsContext(),
               spriteBatch(),
               camera (320, 240)
{
    Logger::getLogger().appendToLog ("Creating Context");
    if (! SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS))
    {
        Logger::getLogger().appendToLog ("SDL CONTEXT Creation failed: ", SDL_GetError());
        exit (-1);
    }

    graphicsContext.initContext();

    if (! graphicsContext.setRenderTextureSize (320, 240))
    {
        Logger::log ("failed to create texture size of:", 320, " ", 240);
    }

    spriteBatch.init (&graphicsContext);

    tileRenderer.init (&graphicsContext);

    ldtk::Project project;
    project.loadFromFile ("assets/levels/GdungeonCrawler.ldtk");

    graphicsContext.getTextureManager()->beginBatchUpload (&graphicsContext);
    auto tex = graphicsContext.getTextureManager()->loadTexture ("ravioli_atlas.bmp");
    tileRenderer.preLoadTextures (project, &graphicsContext);

    sprite = Sprite (tex, 0, 0, 16, 16, 0, 0, 16, 16);

    graphicsContext.getTextureManager()->endBatchUpload (&graphicsContext);
    tileRenderer.loadTileMap (project.getWorld().getLevel (0), &graphicsContext);

    ImGUIHelpers::initContext (&graphicsContext);
    running = true;
}

void Game::run()
{
    lastFrameTime = currentFrameTime;
    currentFrameTime = SDL_GetTicks();
    float delta = (currentFrameTime - lastFrameTime) / 1000;
    while (running)
    {
        handleEvents (delta);
        render();
    }
}

void Game::handleEvents (float)
{
    SDL_Event event;
    while (SDL_PollEvent (&event))
    {
        ImGUIHelpers::handleEvents (&event);
        switch (event.type)
        {
            case SDL_EVENT_QUIT:
                running = false;
                //#TODO create more sophisticated running shutdown logic
                break;
            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_TAB)
                {
                    showEditor = ! showEditor;
                }
            default:
                break;
        }
    }

    const bool* keyStates = SDL_GetKeyboardState (NULL);

    auto speed = 1; //(0.5 * delta);
    if (keyStates[SDL_SCANCODE_LEFT])
    {
        camera.setX (camera.getX() - speed);
    }
    if (keyStates[SDL_SCANCODE_RIGHT])
    {
        camera.setX (camera.getX() + speed);
    }
    if (keyStates[SDL_SCANCODE_UP])
    {
        camera.setY (camera.getY() - speed);
    }
    if (keyStates[SDL_SCANCODE_DOWN])
    {
        camera.setY (camera.getY() + speed);
    }
}

void Game::drawEditor()
{
    ImGui::Begin ("Graphics Context");

    auto io = ImGui::GetIO();
    ImGui::Text ("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::Text ("Driver Name: %s", SDL_GetGPUDeviceDriver (graphicsContext.getDevice()));
    auto properties = SDL_GetGPUDeviceProperties (graphicsContext.getDevice());
    (SDL_EnumerateProperties (properties, [] (void*, SDL_PropertiesID prop, const char* name)
                              {
        ImGui::Text ("%s", name);
        ImGui::SameLine();
        ImGui::GetWindowDrawList()->AddCallback (ImDrawCallback_ImplSDLGPU3_SetSampler, nullptr);
        ImGui::Text ("%s", SDL_GetStringProperty (prop, name, "")); },
                              nullptr));

    ImGui::End();

    ImGui::Begin ("GameWindow");
    ImGui::GetWindowDrawList()->AddCallback (ImDrawCallback_ImplSDLGPU3_SetSampler, nullptr);
    // auto windowWidth = ImGui::GetWindowSize().x;
    // auto windowHeight = ImGui::GetWindowSize().y;
    // {
    //     ImGui::SetCursorPos ({ (windowWidth - 640) / 2, (windowHeight - 480) / 2 });
    // }

    ImGui::End();
}

void Game::render()
{
    graphicsContext.startFrame();
    //#TODO put proper interface in here for this.
    graphicsContext.getFrameContext()->cameraMatrix = camera.getCurrentMatrix();
    // graphicsContext.startRenderTexture();
    tileRenderer.draw (graphicsContext.getFrameContext());

    spriteBatch.draw (&sprite);

    spriteBatch.render (graphicsContext.getFrameContext());
    graphicsContext.endRenderTexture();

    if (showEditor)
    {
        ImGUIHelpers::startFrame();
        drawEditor();
        ImGUIHelpers::endFrame (graphicsContext.getFrameContext());
    }

    graphicsContext.endFrame();
}

void Game::shutdown()
{
    SDL_DestroyWindow (window);
}
