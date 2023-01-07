cmake_minimum_required (VERSION 3.2)

project(openXLSX)

add_definitions(-D OPENXLSX_STATIC_DEFINE)

set(OPENXLSX_SRC
	XLCell.cpp
	XLCellIterator.cpp
	XLCellRange.cpp
	XLCellReference.cpp
	XLCellValue.cpp
	XLColor.cpp
	XLColumn.cpp
	XLContentTypes.cpp
	XLDocument.cpp
	XLProperties.cpp
	XLRelationships.cpp
	XLRow.cpp
	XLSharedStrings.cpp
	XLSheet.cpp
	XLWorkbook.cpp
	XLXmlData.cpp
	XLXmlFile.cpp
	XLZipArchive.cpp
)

include_directories(
  ./
  ../zippy/
  ../
  ../pugixml/
)

# 生成静态库文件 通过指定源文件来生成库文件
add_library(openXLSX STATIC
  ${OPENXLSX_SRC}
)

# 设置库文件属性
set_target_properties(openXLSX
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

# 设置该库依赖pugixml库
target_link_libraries(openXLSX pugixml) 

