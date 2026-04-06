cmake_minimum_required(VERSION 3.28)


project("Game"
    VERSION 1.0
    DESCRIPTION "SDL Top Down"
    LANGUAGES C CXX
    )

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC
    src/
)

target_sources(${PROJECT_NAME} 
    PUBLIC
    src/main.cpp
    src/game.cpp
)

include(../asset_copy.cmake)

target_link_libraries(${PROJECT_NAME} PRIVATE Olympus)
