
macro(asset_copy)
  add_custom_target("${PROJECT_NAME}_ASSET_COPY"
    COMMAND ${CMAKE_COMMAND} -E echo "Copying Assets..."
    COMMAND ${CMAKE_COMMAND} -E remove_directory .local/suare/{PROJECT_NAME}/assets/
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ~/.local/share/${PROJECT_NAME}/assets/)



  add_dependencies(${PROJECT_NAME} "${PROJECT_NAME}_ASSET_COPY")


endmacro(asset_copy)

asset_copy()
