cmake_minimum_required(VERSION 3.28)


project("Concorde"
    VERSION 1.0
    DESCRIPTION "SDL Top Down"
    LANGUAGES C CXX OBJC
    )

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE)



target_sources(${PROJECT_NAME} 
    PUBLIC
        src/main.cpp
        src/mainWindow.cpp

        src/EditorWindows/sceneEditor.cpp
        src/EditorWindows/editorSettings.cpp
        src/EditorWindows/projectSettings.cpp
    
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

fetchContent_Declare(
    nfd
    GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
    GIT_TAG fc168e8605bfa51aaec22ab0c4e46b9de665a437
)
FetchContent_MakeAvailable(nfd)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE 
    Olympus
    nfd
)

