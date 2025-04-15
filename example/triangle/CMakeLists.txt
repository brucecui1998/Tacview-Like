cmake_minimum_required(VERSION 3.15)

add_executable(triangle main.cpp)

# 链接 OpenGL + GLEW + GLFW
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(triangle ${OPENGL_gl_LIBRARY})

# GLEW (源码方式编译)
set(GLEW_SRC "${CMAKE_SOURCE_DIR}/external/glew/src/glew.c")
include_directories(${CMAKE_SOURCE_DIR}/external/glew/include)
add_definitions(-DGLEW_STATIC)  # 必须定义这个以避免链接动态库
target_sources(triangle PRIVATE ${GLEW_SRC})

# GLFW
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    include_directories(${GLFW_ROOT}/debug/include)
    set(GLFW_LIB "${GLFW_ROOT}/debug/lib/glfw3dll.lib")
    set(GLFW_DLL_DEBUG_PATH "${GLFW_ROOT}/debug/bin")
else()
    include_directories(${GLFW_ROOT}/release/include)
    set(GLFW_LIB "${GLFW_ROOT}/release/lib/glfw3dll.lib")
    set(GLFW_DLL_RELEASE_PATH "${GLFW_ROOT}/release/bin")
endif()

target_link_libraries(triangle ${GLFW_LIB})

# 拷贝glfw3.dll
add_custom_command(TARGET triangle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<$<CONFIG:Debug>:${GLFW_ROOT}/debug/bin/glfw3.dll>
        $<$<CONFIG:Release>:${GLFW_ROOT}/release/bin/glfw3.dll>
        $<TARGET_FILE_DIR:triangle>)

