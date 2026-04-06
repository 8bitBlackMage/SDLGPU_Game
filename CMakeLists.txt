cmake_minimum_required(VERSION 3.23)

Project("DungeonCrawler"
    VERSION 1.0
    DESCRIPTION "SDL Top Down"
    LANGUAGES C CXX
    )

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)


add_subdirectory(Engine)

# add_dependencies(${PROJECT_NAME} ASSET_COPY)

# if(MSVC)
#   target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
# else()
#   target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
# endif()




add_subdirectory(Editor)

add_subdirectory(Game)



