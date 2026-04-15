cmake_minimum_required(VERSION 3.28)


project("Concorde"
    VERSION 1.0
    DESCRIPTION "SDL Top Down"
    LANGUAGES C CXX
    )

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE)



target_sources(${PROJECT_NAME} 
    PUBLIC
    src/main.cpp
    src/mainWindow.cpp
    src/sceneEditor.cpp
    src/editorSettings.cpp
    
)


if(APPLE)
set(MACOSX_BUNDLE_ICON_FILE "icon.icns")
set(application_icon "assets/${MACOSX_BUNDLE_ICON_FILE}")
set_source_files_properties(${application_icon}
    PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
)
set(info_plist "assets/Info.plist")
set_source_files_properties(${info_plist}
    PROPERTIES
        MACOSX_PACKAGE_LOCATION "/"
)
target_sources(${PROJECT_NAME} 
    PUBLIC
    ${application_icon}
    ${info_plist}
)
endif()


include(../asset_copy.cmake)



target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE 
    Olympus
)

