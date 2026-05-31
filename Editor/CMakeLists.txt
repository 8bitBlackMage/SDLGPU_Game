cmake_minimum_required(VERSION 3.28)

include(../cmake/project_function.cmake)

OLYMPUS_PROJECT(Concorde 1.0 "Editor for Olympus Engine")



target_sources(${PROJECT_NAME}
        PUBLIC
        src/main.cpp
        src/mainWindow.cpp

        src/EditorWindows/sceneEditor.cpp
        src/EditorWindows/editorSettings.cpp
        src/EditorWindows/projectSettings.cpp
)




if (APPLE)
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
endif ()



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

