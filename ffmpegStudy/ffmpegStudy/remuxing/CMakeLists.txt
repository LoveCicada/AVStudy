
message("STATUS" ${CMAKE_CURRENT_SOURCE_DIR})
set(NAME_BINART ${CMAKE_CURRENT_SOURCE_DIR})
project(remuex)

aux_source_directory("./" DIR_SRCS)

add_executable(remuex ${DIR_SRCS})

# include
include_directories(${INCLUDE_PATH})

# 链接对应的库
target_link_directories(remuex PRIVATE "${LIBRARY_OUTPUT_PATH}")

# 链接对应的依赖库
target_link_libraries(remuex PRIVATE avcodec.lib avdevice.lib avfilter.lib avformat.lib avutil.lib postproc.lib swresample.lib swscale.lib)

