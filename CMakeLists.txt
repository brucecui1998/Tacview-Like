cmake_minimum_required(VERSION 3.15)
project(TacViewLike)

set(CMAKE_CXX_STANDARD 17)

# 添加子模块头文件路径
include_directories(
    ${CMAKE_SOURCE_DIR}/src
)

if(NOT DEFINED Qt5_DIR)
    set(Qt5_DIR "D:/Software/qt5.15.10-win64-debug-release-msvc2022-shared/msvc64/lib/cmake/Qt5")
endif()
find_package(Qt5 REQUIRED COMPONENTS Widgets OpenGL)
# 提取 Qt 的 bin 路径（基于 Qt5_DIR）
get_filename_component(QT5_BASE_DIR "${Qt5_DIR}/../../.." ABSOLUTE)
set(QT5_BIN_DIR "${QT5_BASE_DIR}/bin")

# 查找 OpenGL
find_package(OpenGL REQUIRED)

# 设置输出路径到 bin 目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>)



# 源码路径
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${SOURCE_DIR}/*.h)

# 添加 tinyobjloader 源码
set(SRC_LIB
    src/thirdparty/tiny_obj_loader/tiny_obj_loader.cc
)

# Qt 包含目录
set(CMAKE_AUTOMOC ON) # 处理 Q_OBJECT 宏
set(CMAKE_AUTORCC ON) # 编译 .qrc 资源文件，如图片、样式等
set(CMAKE_AUTOUIC ON) # 将 .ui 文件转为 .h/.cpp，生成界面布局代码



# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${SRC_LIB})

# 添加 win_my_grep 子项目（路径根据实际情况）
add_subdirectory(tools/win_my_grep)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
    # 设置 Visual Studio 调试时的工作目录
    set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>"
    )
endif()



# 链接库
target_link_libraries(${PROJECT_NAME} 
    Qt5::Widgets
    Qt5::OpenGL
    OpenGL::GL
    OpenGL::GLU  # 添加这行
)

# 添加 Qt bin 路径到运行时 PATH（避免复制 DLL）
# 注意：这仅在编译器支持运行时 PATH 的平台有效
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES 
    VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${QT5_BIN_DIR}")
endif()

# 设置资源目录和目标目录
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")
set(OUTPUT_RESOURCE_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")

# 拷贝资源目录到 bin/Debug 或 bin/Release
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${RESOURCE_DIR}" "${OUTPUT_RESOURCE_DIR}"
)


# 添加子项目example
set(GLFW_ROOT "${CMAKE_SOURCE_DIR}/external/GLFW-3.4")
set(GLEW_ROOT "${CMAKE_SOURCE_DIR}/external/glew")
add_subdirectory(example/triangle)