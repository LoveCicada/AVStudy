
message("STATUS" ${CMAKE_CURRENT_SOURCE_DIR})
set(NAME_BINART ${CMAKE_CURRENT_SOURCE_DIR})

# @see CMake获取当前目录名以及获取上层目录名
# @http https://blog.csdn.net/FlushHip/article/details/114677610

# remove last end of "/"
string(REGEX REPLACE "/$" "" CURRENT_FOLDER_ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR})
# get current relative dir name and set target name
string(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CURRENT_FOLDER_ABSOLUTE})
set (TARGET_NAME ${CURRENT_FOLDER})

get_filename_component(CURRENT_FOLDER ${CURRENT_FOLDER_ABSOLUTE} NAME)
project(${TARGET_NAME})

aux_source_directory("./" DIR_SRCS)

add_executable(${TARGET_NAME} ${DIR_SRCS})

# include
include_directories(${INCLUDE_PATH})

# 链接对应的库
target_link_directories(${TARGET_NAME} PRIVATE "${LIBRARY_OUTPUT_PATH}")

# 链接对应的依赖库
target_link_libraries(${TARGET_NAME} PRIVATE avcodec 
                     avdevice 
                     avfilter 
                     avformat 
                     avutil 
                     postproc 
                     swresample 
                     swscale)

