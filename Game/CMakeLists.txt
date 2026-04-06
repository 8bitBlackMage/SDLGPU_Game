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
    src/editor.cpp
)

add_custom_target(ASSET_COPY
    COMMAND ${CMAKE_COMMAND} -E echo "Copying Assets..."
    COMMAND ${CMAKE_COMMAND} -E remove_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
add_dependencies(${PROJECT_NAME} ASSET_COPY)


target_link_libraries(${PROJECT_NAME} PRIVATE Olympus)
