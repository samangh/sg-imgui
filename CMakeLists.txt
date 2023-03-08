function(imgui_get_src_files)
  set(options "")
  set(multiValueArgs "")
  set(oneValueArgs DIR)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  set(IMGUI_WRAPPER_DIR ${ARG_DIR} CACHE INTERNAL "IMGUI_WRAPPER_DIR")
  set(IMGUI_DIR ${ARG_DIR}/external/imgui CACHE INTERNAL "IMGUI_DIR")
  set(IMPLOT_DIR ${ARG_DIR}/external/implot CACHE INTERNAL "IMPLOT_DIR")
  set(IMGUI_FILE_BROWSER_DIR ${ARG_DIR}/external/imgui-filebrowser CACHE INTERNAL "IMGUI_FILE_BROWSER_DIR")

  find_package(SDL2 REQUIRED)
  find_package(OpenGL REQUIRED)

  ##
  ## Source files
  ##

  ## ImGui source files
  ##
  file(GLOB IMGUI_SRC_FILES
    ${IMGUI_DIR}/*.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp)

  ## ImPlot
  ##
  file(GLOB IMPLOT_SRC_FILES
    ${IMPLOT_DIR}/*.cpp)
  #list(REMOVE_ITEM IMPLOT_SRC_FILES ${IMPLOT_DIR}/implot_demo.cpp)

  ## Helper source files
  file(GLOB_RECURSE IMGUI_WRAPPER_OWN_SRC_FILES
    ${ARG_DIR}/src/*.c
    ${ARG_DIR}/src/*.cc
    ${ARG_DIR}/src/*.cpp)

  set(IMGUI_WRAPPER_SRC_FILES ${IMGUI_WRAPPER_OWN_SRC_FILES} ${IMGUI_SRC_FILES} ${IMPLOT_SRC_FILES} PARENT_SCOPE)
endfunction()

##
## Includes
##

function(imgui_add_to_target)
  set(options "")
  set(multiValueArgs "")
  set(oneValueArgs TARGET)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  # Use our version of imconfig.h
  target_compile_definitions(${ARG_TARGET} PRIVATE IMGUI_DISABLE_INCLUDE_IMCONFIG_H)
  target_compile_definitions(${ARG_TARGET} PRIVATE IMGUI_USER_CONFIG="udaq/wrappers/imgui/config.h")

  target_include_directories(${ARG_TARGET}
    PRIVATE
      ${IMGUI_WRAPPER_DIR}/include
      ${IMGUI_DIR}

      ${IMPLOT_DIR}
      ${IMGUI_FILE_BROWSER_DIR})

  ##
  ## SDL
  ##

  if(TARGET SDL2::SDL2main)
    if(TARGET SDL2::SDL2-static)
      if (NOT BUILD_SHARED_LIBS OR NOT TARGET SDL2::SDL2)
        set(USE_STATIC_SDL ON)
      endif()
    endif()
    target_link_libraries(${ARG_TARGET} PRIVATE
      $<IF:$<BOOL:${USE_STATIC_SDL}>,SDL2::SDL2-static,SDL2::SDL2>
      SDL2::SDL2main)
  else()
    target_include_directories(${ARG_TARGET} PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(${ARG_TARGET} PRIVATE ${SDL2_LIBRARIES})
  endif()

  ##
  ## Linking
  ##

  target_link_libraries(${ARG_TARGET}
    PRIVATE
      OpenGL::GL
      "$<$<BOOL:${APPLE}>:-framework CoreFoundation>"
      $<$<AND:$<BOOL:${UNIX}>,$<NOT:$<BOOL:${APPLE}>>>:${CMAKE_DL_LIBS}>)
endfunction()
