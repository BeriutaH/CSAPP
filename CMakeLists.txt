cmake_minimum_required(VERSION 3.27)
project(CsappPro C)

set(CMAKE_C_STANDARD 17)

# 使用 GLOB_RECURSE 查找所有 .c 和 .h 文件
file(GLOB_RECURSE SOURCES "exercises/**/*.c" "main.c")
file(GLOB_RECURSE HEADERS "exercises/**/*.h")

# 将所有源文件和头文件添加到可执行文件
add_executable(CsappPro ${SOURCES} ${HEADERS})
