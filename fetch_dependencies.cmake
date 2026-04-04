include(fetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
    GIT_TAG "main"
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
    SDL3_Image
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL_image"
    GIT_TAG "main"
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(SDL3_Image)


FetchContent_Declare(
    SDL3_Mixer
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL_mixer"
    GIT_TAG "main"
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(SDL3_Mixer)

FetchContent_Declare(
    SDL3_Ttf
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL_ttf"
    GIT_TAG "main"
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(SDL3_Ttf)


FetchContent_Declare(
    rectpack2D
    GIT_REPOSITORY "https://github.com/TeamHypersomnia/rectpack2D.git"
    GIT_TAG "master"
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(rectpack2D)

FetchContent_Declare(
    LDtkLoader
    GIT_REPOSITORY "https://github.com/Madour/LDtkLoader"
    GIT_TAG "Master"
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(LDtkLoader)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
    GIT_TAG "docking"
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(imgui)

add_library(imgui)

target_include_directories(imgui 
    PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_sources(imgui
    PUBLIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlgpu3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

target_link_libraries(imgui PUBLIC SDL3::SDL3)


add_library(dependencies INTERFACE)

target_link_libraries(dependencies INTERFACE 
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    rectpack2D
    imgui
    LDtkLoader::LDtkLoader
)