cmake_minimum_required(VERSION 3.23)



Project("DungeonCrawler"
    VERSION 1.0
    DESCRIPTION "SDL Top Down"
    LANGUAGES C CXX
    )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    src/
)

target_sources(${PROJECT_NAME}
    PUBLIC
    src/main.cpp
    src/game.cpp
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