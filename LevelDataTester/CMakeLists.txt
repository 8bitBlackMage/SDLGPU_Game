cmake_minimum_required(VERSION 3.28)


project("levelDataTester"
        LANGUAGES C CXX)


add_executable(${PROJECT_NAME})


target_sources(${PROJECT_NAME}
        PUBLIC
        src/main.cpp)


include(../cmake/asset_copy.cmake)

target_link_libraries(${PROJECT_NAME} PRIVATE Olympus)

