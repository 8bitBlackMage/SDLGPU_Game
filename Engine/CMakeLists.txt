cmake_minimum_required(VERSION 3.28)


Project("Olympus"
    VERSION 1.0
    LANGUAGES C CXX)


add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    src/
)

target_sources(${PROJECT_NAME}
    PUBLIC
    src/Utils/logger.cpp
    src/Graphics/graphicsContext.cpp
    src/Graphics/camera.cpp
    src/Graphics/renderTexture.cpp
    src/Graphics/Textures/textureManager.cpp
    src/Graphics/TileMap/tileMapRenderer.cpp
    src/Graphics/Textures/texture.cpp
    src/Graphics/Pipelines/spriteBatch.cpp
    src/Graphics/Pipelines/LineRenderer.cpp
    src/Graphics/Structs/sprite.cpp
    src/Graphics/Structs/line.cpp

    src/LevelData/Maps/tileLayer.cpp
    src/Imgui/imguiHelpers.cpp
)

include(../fetch_dependencies.cmake)





target_link_libraries(${PROJECT_NAME}
    PUBLIC
    dependencies
    )