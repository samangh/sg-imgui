set(TARGET "imgui-helper")
set_property(GLOBAL APPEND PROPERTY ${PROJECT_NAME}_TARGETS ${TARGET})

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

set(IMGUI_DIR ${CMAKE_CURRENT_LIST_DIR}/external/imgui)
set(IMPLOT_DIR ${CMAKE_CURRENT_LIST_DIR}/external/implot)
set(IMGUI_FILE_BROWSER_DIR ${CMAKE_CURRENT_LIST_DIR}/external/imgui-filebrowser)

##
## Source files
##

## ImGui source files
##
file(GLOB IMGUI_SRC_FILES
  ${IMGUI_DIR}/*.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
  ${IMGUI_DIR}/backends/imgui_impl_sdl.cpp)

## ImPlot
##
file(GLOB IMPLOT_SRC_FILES
  ${IMPLOT_DIR}/*.cpp)
list(REMOVE_ITEM IMPLOT_SRC_FILES ${IMPLOT_DIR}/implot_demo.cpp)

## Helper source files
file(GLOB_RECURSE SRC_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(${TARGET} STATIC ${SRC_FILES} ${IMGUI_SRC_FILES} ${IMPLOT_SRC_FILES})

##
## Includes
##

# Use our version of imconfig.h
target_compile_definitions(${TARGET} PUBLIC IMGUI_DISABLE_INCLUDE_IMCONFIG_H)
target_compile_definitions(${TARGET} PUBLIC IMGUI_USER_CONFIG="udaq/wrappers/imgui/config.h")

target_include_directories(${TARGET}
  INTERFACE
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>

    $<BUILD_INTERFACE:${IMGUI_DIR}>

    $<BUILD_INTERFACE:${IMPLOT_DIR}>
    $<BUILD_INTERFACE:${IMGUI_FILE_BROWSER_DIR}>
  PRIVATE)

##
## SDL
##

if(TARGET SDL2::SDL2main)
  if(TARGET SDL2::SDL2-static)
    if (NOT BUILD_SHARED_LIBS OR NOT TARGET SDL2::SDL2)
      set(USE_STATIC_SDL ON)
    endif()
  endif()

  target_link_libraries(${TARGET} PUBLIC
      $<IF:$<BOOL:${USE_STATIC_SDL}>,SDL2::SDL2-static,SDL2::SDL2>
      SDL2::SDL2main)
else()
  target_include_directories(${TARGET} PUBLIC ${SDL2_INCLUDE_DIRS})
  target_link_libraries(${TARGET} PUBLIC ${SDL2_LIBRARIES})
endif()

##
## Linking
##

target_link_libraries(${TARGET}
  INTERFACE
  PUBLIC
  PRIVATE
    OpenGL::GL
    "$<$<BOOL:${APPLE}>:-framework CoreFoundation>"
    $<$<AND:$<BOOL:${UNIX}>,$<NOT:$<BOOL:${APPLE}>>>:${CMAKE_DL_LIBS}>)

#setup_ide_folders(TARGET ${TARGET} DIR ${CMAKE_CURRENT_SOURCE_DIR})

##
## Compiler flags
##

target_compile_features(${TARGET} PRIVATE cxx_std_17)

##
## Version
##

configure_file (
  "${PROJECT_SOURCE_DIR}/cmake/version.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h"
  )

