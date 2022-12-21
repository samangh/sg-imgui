set(TARGET "imgui")
set_property(GLOBAL APPEND PROPERTY ${PROJECT_NAME}_TARGETS ${TARGET})

set(IMGUI_EXT_DIR ${CMAKE_CURRENT_LIST_DIR}/../external/imgui)
set(IMGUI_DIR ${CMAKE_CURRENT_LIST_DIR}/src_temp)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

##
## ImGUI - Source files
##

file(COPY ${IMGUI_EXT_DIR}/ DESTINATION ${IMGUI_DIR}
  FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
    PATTERN "*.c"
    PATTERN "*.cpp"
    PATTERN "imconfig.h" EXCLUDE
    PATTERN "imgui_demo.cpp" EXCLUDE)
    
file(GLOB IMGUI_SRC_FILES
  ${IMGUI_DIR}/*.cpp
  ${IMGUI_DIR}/examples/libs/gl3w/GL/*.c
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
  ${IMGUI_DIR}/backends/imgui_impl_sdl.cpp)

##
## Library
##

add_library(${TARGET} STATIC ${IMGUI_SRC_FILES} ${IMPLOT_SRC_FILES})

##
## Includes
##

target_include_directories(${TARGET}
  INTERFACE
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<BUILD_INTERFACE:${IMGUI_DIR}>
    $<BUILD_INTERFACE:${IMGUI_DIR}/examples/libs/gl3w>
  PRIVATE)

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


target_link_libraries(${TARGET}
  INTERFACE
  PUBLIC
  PRIVATE
    OpenGL::GL
    "$<$<BOOL:${APPLE}>:-framework CoreFoundation>"
    $<$<AND:$<BOOL:${UNIX}>,$<NOT:$<BOOL:${APPLE}>>>:${CMAKE_DL_LIBS}>)


setup_ide_folders(TARGET ${TARGET} DIR ${CMAKE_CURRENT_SOURCE_DIR})

##
## Compiler flags
##

target_compile_features(${TARGET} PRIVATE cxx_std_17)

##
## Version
##

configure_file (
  "${PROJECT_SOURCE_DIR}/cmake/version.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h")

