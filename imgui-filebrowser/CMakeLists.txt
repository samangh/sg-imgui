set(TARGET "imgui-filebrowser")

##
## Source files
##

add_library(${TARGET} INTERFACE)

##
## Includes
##

target_include_directories(${TARGET}
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../external/imgui-filebrowser>
    imgui)

