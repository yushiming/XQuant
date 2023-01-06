
cmake_minimum_required (VERSION 3.2)

project(pugixml)

set(PUGIXML_SRC
	pugixml.cpp
)

include_directories(
  ./
)

add_library(pugixml STATIC
  ${PUGIXML_SRC}
)

set_target_properties(pugixml
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)
