cmake_minimum_required(VERSION 3.28)

include(../cmake/project_function.cmake)

OLYMPUS_PROJECT(TESTGAME 0.1 "TEST GAME")


target_sources(${PROJECT_NAME}
    PUBLIC
    src/main.cpp
    src/game.cpp
)



target_link_libraries(${PROJECT_NAME} PRIVATE Olympus)
