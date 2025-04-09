cmake_minimum_required(VERSION 3.15)

# 设置项目名（可以随便命名）
project(win_my_grep)

# 添加可执行文件
add_executable(win_my_grep win_my_grep.cpp)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
    # 设置 Visual Studio 调试时的工作目录
    set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>"
    )
endif()

# 设置输出目录为 ../../bin（即根目录下的 bin 文件夹）
set_target_properties(win_my_grep PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>
)
