
message("STATUS" ${CMAKE_CURRENT_SOURCE_DIR})
set(NAME_BINART ${CMAKE_CURRENT_SOURCE_DIR})
project(ffplay)

aux_source_directory("./" DIR_SRCS)

add_executable(ffplay ${DIR_SRCS})

# include
include_directories(${INCLUDE_PATH})

# 链接对应的库
target_link_directories(ffplay PRIVATE "${LIBRARY_OUTPUT_PATH}")

# 链接对应的依赖库
target_link_libraries(ffplay PRIVATE avcodec 
                    avdevice 
                    avfilter 
                    avformat
                    avutil
                    postproc
                    swresample
                    swscale
                    SDL2)

