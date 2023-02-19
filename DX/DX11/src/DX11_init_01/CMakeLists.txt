

cmake_minimum_required(VERSION 3.10)

make_project_name(TARGET_NAME)
message("STATUS" ${TARGET_NAME})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_src(SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC_FILES})

if(WIN32)
    set(WINRC ${CMAKE_CURRENT_SOURCE_DIR}/resource/rc)
    add_executable(${TARGET_NAME} WIN32 ${SRC_FILES} ${WINRC}/app.rc)
    set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    # 指定C++ 异常处理程序try()catch的展开语义
    set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_FLAGS "/EHsc")
    target_compile_definitions(${TARGET_NAME} PRIVATE
        NOMINMAX
    )
endif()

message(TRACE "${TARGET_NAME} ${SRC_FILES}")

target_link_libraries(${TARGET_NAME} PUBLIC
    d3d11.lib
)