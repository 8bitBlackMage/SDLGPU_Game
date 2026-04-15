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

set(MACOSX_BUNDLE_ICON_FILE "icon.icns")
set(application_icon "assets/${MACOSX_BUNDLE_ICON_FILE}")
set_source_files_properties(${application_icon}
    PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES # https://cmake.org/cmake/help/latest/prop_tgt/MACOSX_BUNDLE_INFO_PLIST.html
        MACOSX_BUNDLE_BUNDLE_NAME "${CMAKE_PROJECT_NAME}" # CFBundleIdentifier
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.8bitBlackMage"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION} # CFBundleLongVersionString, deprecated
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION} # CFBundleShortVersionString
        # and bundle resources again
        RESOURCE "${resource_files}"
)

target_sources(${PROJECT_NAME} 
    PUBLIC
    ${application_icon}
    src/main.cpp
    src/mainWindow.cpp
    src/sceneEditor.cpp
    src/editorSettings.cpp
    
)

include(../asset_copy.cmake)



target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE 
    Olympus
)

