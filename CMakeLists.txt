cmake_minimum_required(VERSION 3.23)



Project("DungeonCrawler"
    VERSION 1.0
    DESCRIPTION "SDL Top Down"
    LANGUAGES C CXX
    )

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    src/
    src/engine
)

target_sources(${PROJECT_NAME}
    PUBLIC
    src/main.cpp
    src/Engine/game.cpp
    src/Engine/logger.cpp
    src/Engine/Scenes/SceneManager.cpp
    src/Engine/Scenes/testScene.cpp
    src/Engine/Graphics/graphicsContext.cpp
    src/Engine/Graphics/spriteBatch.cpp
    )

set(VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor")
add_subdirectory(${VENDOR_DIR}/SDL)
add_subdirectory(${VENDOR_DIR}/SDL_image)
add_subdirectory(${VENDOR_DIR}/SDL_ttf)
add_subdirectory(${VENDOR_DIR}/imgui)

target_include_directories(${PROJECT_NAME} INTERFACE ${VENDOR_DIR}/imgui)

target_link_libraries(${PROJECT_NAME}
    PRIVATE 
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    imgui
    )

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)