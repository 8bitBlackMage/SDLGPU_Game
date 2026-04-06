cmake_minimum_required(VERSION 3.28)


project("Concorde"
    VERSION 1.0
    DESCRIPTION "SDL Top Down"
    LANGUAGES C CXX
    )

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} 
    PUBLIC
    src/main.cpp
)