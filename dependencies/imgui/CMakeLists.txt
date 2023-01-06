
cmake_minimum_required (VERSION 3.2)

project(imgui)

set(IMGUI_SRC
	imgui.cpp
	imgui_demo.cpp
	imgui_draw.cpp
	imgui_impl_glfw.cpp
	imgui_impl_opengl3.cpp
	imgui_tables.cpp
	imgui_widgets.cpp
)

include_directories(
  ./
  ../glfw-3.3.8.WIN64/include
)

add_library(imgui STATIC
  ${IMGUI_SRC}
)

MESSAGE(STATUS "${CMAKE_CURRENT_SOURCE_DIR}/lib" = ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set_target_properties(imgui
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)
