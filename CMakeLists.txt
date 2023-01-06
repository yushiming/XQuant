# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.2)

# 设置变量名字
set(PROJECT_NAME XQuant)
# 设置使用C++17标准
set(CMAKE_CXX_STANDARD 17)

# 项目信息
project (${PROJECT_NAME})

# 设置Debug和Release编译 默认情况下 CMAKE_BUILD_TYPE为Debug
set(CMAKE_BUILD_TYPE "Debug")

MESSAGE(STATUS "CMAKE_CURRENT_SOURCE_DIR" = ${CMAKE_CURRENT_SOURCE_DIR})

#MESSAGE(STATUS "CMAKE_SOURCE_DIR" = ${CMAKE_SOURCE_DIR})
#MESSAGE(STATUS "CMAKE_BINARY_DIR" = ${CMAKE_BINARY_DIR})
#${PROJECT_NAME}_INCLUDE 为 XQuant_INCLUDE，这个变量是在指定项目名称为XQuant后自动生成的变量
#设置rapidjson库的头文件目录
#set(RAPIDJSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/rapidjson-master/include)
#将上述的头文件目录添加到一个列表变量之中
#list(APPEND ${PROJECT_NAME}_INCLUDE ${RAPIDJSON_INCLUDE_DIR})

#XQuant_LIBS这个变量是在指定项目名称为XQuant后自动生成的变量
#将上述的库文件目录添加到一个列表变量之中
#list(APPEND XQuant_LIBS ${GLEW_LIBRARY} ${GLFW3_LIBRARY} ${OPENGL32_LIBRARY} ${CTP_LIBRARY})

#IF(WIN32)
#	MESSAGE(STATUS "----OS is windows----")
#ELSEIF(UNIX)
#	MESSAGE(STATUS "----OS is UNIX----")
#ENDIF()

# 查找当前src目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
#aux_source_directory(./src/ DIR_SRCS)

# 找到三方静态库
#find_library(jsoncpp_lib NAMES jsoncpp PATHS ./jsoncpp)


#-------- 设置变量值
#设置glew动态库的库文件路径
set(GLEW_LIBRARY_DLL ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glew-2.2.0/bin/Release/x64/glew32.dll)

#设置GLFW动态库的库文件路径
set(GLFW3_LIBRARY_DLL ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw-3.3.8.WIN64/lib-vc2019/glfw3.dll)

#设置ctp动态库的库文件路径
set(CTP_THOSTMDUSERAPI_SE_LIBRARY_DLL ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ctp-6.6.9/lib/thostmduserapi_se.dll)
set(CTP_THOSTTRADERAPI_SE_LIBRARY_DLL ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ctp-6.6.9/lib/thosttraderapi_se.dll)

# 添加dependencies子目录
add_subdirectory(dependencies)


#设置项目的头文件包含目录
include_directories(
	dependencies/rapidjson-master/include
	dependencies/glew-2.2.0/include
	dependencies/glfw-3.3.8.WIN64/include
	dependencies/ctp-6.6.9/include
	dependencies/imgui
)

# include 指令用来载入并运行来自于文件或模块的 CMake 代码
include(${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeLists.txt)

# 指定生成目标
add_executable(${PROJECT_NAME} 
	${SRC_FILE} 
	${CORE_FILE} 
	${DATA_FILE}
	${DEBUG_FILE}
	${EVENTS_FILE}
	${FRAMES_FILE}
	${IMGUIEXT_IMPLOT_FILE}
	${IO_FILE}
	${LANGUAGE_FILE}
	${NET_FILE}
	${PLATFORM_CTP_FILE}
	${PLATFORM_XTP_FILE}
	${STRATEGY_FILE}
	${UTILS_FILE}
)

#设置项目的外部依赖库 具体库名称或库应用名称
target_link_libraries(${PROJECT_NAME} 
	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glew-2.2.0/lib/Release/x64/glew32s.lib
	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw-3.3.8.WIN64/lib-vc2019/glfw3.lib
	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ctp-6.6.9/lib/thostmduserapi_se.lib
	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ctp-6.6.9/lib/thosttraderapi_se.lib
	opengl32.lib
	imgui
	openXLSX
	#${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui/lib/${CMAKE_BUILD_TYPE}/imgui.lib
	#${CMAKE_CURRENT_SOURCE_DIR}/dependencies/openXLSX/lib/${CMAKE_BUILD_TYPE}/openXLSX.lib
)

# 创建项目的分组
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET imgui PROPERTY FOLDER "dependencies")
set_property(TARGET openXLSX PROPERTY FOLDER "dependencies")
set_property(TARGET pugixml PROPERTY FOLDER "dependencies")

	
#-------- 目录及文件操作
# 设置可执行程序输出路径
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# 创建输出文件路径
execute_process( COMMAND ${CMAKE_COMMAND} -E make_directory ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE})

#拷贝GLFW3_LIBRARY_DLL 等 dll文件到.exe所在的同级目录下
execute_process(COMMAND ${CMAKE_COMMAND} -E copy "${GLEW_LIBRARY_DLL}" "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/")
execute_process(COMMAND ${CMAKE_COMMAND} -E copy "${GLFW3_LIBRARY_DLL}" "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/")
execute_process(COMMAND ${CMAKE_COMMAND} -E copy "${CTP_THOSTMDUSERAPI_SE_LIBRARY_DLL}" "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/")
execute_process(COMMAND ${CMAKE_COMMAND} -E copy "${CTP_THOSTTRADERAPI_SE_LIBRARY_DLL}" "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/")

