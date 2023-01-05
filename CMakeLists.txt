# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.2)

# 设置变量名字
set(PROJECT_NAME XQuant)

# 项目信息
project (${PROJECT_NAME})

MESSAGE(STATUS "CMAKE_SOURCE_DIR" = ${CMAKE_SOURCE_DIR})
MESSAGE(STATUS "CMAKE_BINARY_DIR" = ${CMAKE_BINARY_DIR})
MESSAGE(STATUS "CMAKE_CURRENT_SOURCE_DIR" = ${CMAKE_CURRENT_SOURCE_DIR})

#设置rapidjson库的头文件目录
set(RAPIDJSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/rapidjson-master/include)

#设置glew库的头文件目录
set(GLEW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/glew-2.2.0/include)
#设置glew库的库文件路径
set(GLEW_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/3rd/glew-2.2.0/lib/Release/x64/glew32s.lib)
#设置glew动态库的库文件路径
#set(GLEW_LIBRARY_DLL ${CMAKE_CURRENT_SOURCE_DIR}/3rd/glew-2.2.0/bin/Release/x64/glew32.dll)

#设置glfw库的头文件目录
set(GLFW3_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/glfw-3.3.8.WIN64/include)
#设置GLFW库的库文件路径
set(GLFW3_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/3rd/glfw-3.3.8.WIN64/lib-vc2019/glfw3.lib)
#设置GLFW动态库的库文件路径
#set(GLFW3_LIBRARY_DLL ${CMAKE_CURRENT_SOURCE_DIR}/3rd/glfw-3.3.8.WIN64/lib-vc2019/glfw3.dll)

#设置opengl32.lib
set(OPENGL32_LIBRARY opengl32.lib)

#XQuant_INCLUDE这个变量是在指定项目名称为XQuant后自动生成的变量
#将上述的头文件目录添加到一个列表变量之中
list(APPEND XQuant_INCLUDE ${RAPIDJSON_INCLUDE_DIR})
list(APPEND XQuant_INCLUDE ${GLEW_INCLUDE_DIR})
list(APPEND XQuant_INCLUDE ${GLFW3_INCLUDE_DIR})

#XQuant_LIBS这个变量是在指定项目名称为XQuant后自动生成的变量
#将上述的库文件目录添加到一个列表变量之中
list(APPEND XQuant_LIBS ${GLEW_LIBRARY})
list(APPEND XQuant_LIBS ${GLFW3_LIBRARY})
list(APPEND XQuant_LIBS ${OPENGL32_LIBRARY})

#设置项目的头文件包含目录
include_directories(${XQuant_INCLUDE})


# 设置ImGui代码文件的路径
set(IMGUI_FILE 
./src/imgui/imconfig.h
./src/imgui/imgui.h
./src/imgui/imgui_impl_glfw.h
./src/imgui/imgui_impl_opengl3.h 
./src/imgui/imgui_impl_opengl3_loader.h
./src/imgui/imgui_internal.h
./src/imgui/imstb_rectpack.h
./src/imgui/imstb_textedit.h
./src/imgui/imstb_truetype.h
./src/imgui/imgui.cpp
./src/imgui/imgui_demo.cpp
./src/imgui/imgui_draw.cpp
./src/imgui/imgui_impl_glfw.cpp
./src/imgui/imgui_impl_opengl3.cpp
./src/imgui/imgui_tables.cpp
./src/imgui/imgui_widgets.cpp)

# 对ImGui的源码分组到ImGui组里
source_group(imgui FILES ${IMGUI_FILE})

# 设置输出文件路径
set(BUILD_OUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/Debug/)
# 创建输出文件路径
execute_process( COMMAND ${CMAKE_COMMAND} -E make_directory ${BUILD_OUT_PATH})

#IF(WIN32)
#	MESSAGE(STATUS "----OS is windows----")
#	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rd/rapidjson-master/include")
#ELSEIF(UNIX)
#	MESSAGE(STATUS "----OS is UNIX----")
#ENDIF()

# 添加3rd子目录
add_subdirectory(3rd)

# add_subdirectory(src)
# 查找当前src目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./src/ DIR_SRCS)

# 指定生成目标
add_executable(${PROJECT_NAME} ${DIR_SRCS} ${IMGUI_FILE})


#拷贝GLFW3_LIBRARY_DLL dll文件到.exe所在的同级目录下
#execute_process(COMMAND ${CMAKE_COMMAND} -E copy "${GLEW_LIBRARY_DLL}" "${BUILD_OUT_PATH}")

#设置项目的外部依赖库
target_link_libraries(${PROJECT_NAME} ${XQuant_LIBS})

