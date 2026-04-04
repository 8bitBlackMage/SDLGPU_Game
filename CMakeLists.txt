cmake_minimum_required(VERSION 3.23)

Project("DungeonCrawler"
    VERSION 1.0
    DESCRIPTION "SDL Top Down"
    LANGUAGES C CXX
    )

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()



target_include_directories(${PROJECT_NAME}
    PUBLIC
    src/
    src/engine
)

target_sources(${PROJECT_NAME}
    PUBLIC
    src/main.cpp
    src/Engine/game.cpp
    src/Engine/Utils/logger.cpp
    src/Engine/Graphics/graphicsContext.cpp
    src/Engine/Graphics/camera.cpp
    src/Engine/Graphics/Textures/textureManager.cpp
    src/Engine/Graphics/TileMap/tileMapRenderer.cpp
    src/Engine/Graphics/Textures/texture.cpp
    src/Engine/Graphics/Sprites/spriteBatch.cpp
    src/Engine/Graphics/Sprites/sprite.cpp
    src/Engine/Utils/matricies.cpp
    )


include(fetch_dependencies.cmake)

target_link_libraries(${PROJECT_NAME} PRIVATE dependencies )

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)