# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.2)

# 设置变量名字
set(PROJECT_NAME XQuant)

# 项目信息
project (${PROJECT_NAME})

MESSAGE(STATUS "CMAKE_SOURCE_DIR" = ${CMAKE_SOURCE_DIR})
MESSAGE(STATUS "CMAKE_BINARY_DIR" = ${CMAKE_BINARY_DIR})
MESSAGE(STATUS "CMAKE_CURRENT_SOURCE_DIR" = ${CMAKE_CURRENT_SOURCE_DIR})

# 添加 math 子目录
add_subdirectory(3rd)

#IF(WIN32)
#	MESSAGE(STATUS "----OS is windows----")
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rd/rapidjson-master/include")
#ELSEIF(UNIX)
#	MESSAGE(STATUS "----OS is UNIX----")
#ENDIF()

# 查找当前src目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./src/ DIR_SRCS)

# 指定生成目标
add_executable(${PROJECT_NAME} ${DIR_SRCS})



