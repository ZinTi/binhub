cmake_minimum_required(VERSION 3.20)
project(binhub VERSION 0.0.1)  # 版本号

# 定义统一版本号变量（所有子项目共享）
set(BINHUB_VERSION ${PROJECT_VERSION})

# 设置所有库和可执行文件的默认输出目录（若子项目没有设置输出目录，则使用此默认目录）
if(WIN32)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/install/${PROJECT_NAME}-v${PROJECT_VERSION}-x86_64-windows/lib) # 静态库 (.a, .lib) 和 导入库 (.dll.a, .lib)
    # set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/install/${PROJECT_NAME}-v${PROJECT_VERSION}-x86_64-windows/bin) # 对 Windows 无效
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/install/${PROJECT_NAME}-v${PROJECT_VERSION}-x86_64-windows/bin) # 运行时文件 (.exe, .dll)
elseif (UNIX)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/install/${PROJECT_NAME}-v${PROJECT_VERSION}-x86_64-linux-gnu/lib) # 静态库 (.a)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/install/${PROJECT_NAME}-v${PROJECT_VERSION}-x86_64-linux-gnu/bin) # 动态库 (.so)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/install/${PROJECT_NAME}-v${PROJECT_VERSION}-x86_64-linux-gnu/bin) # 可执行文件
else()
    message(WARNING "Unsupported platform")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/install/unknown)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/install/unknown)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/libbinhub/unknown)
endif()

# 引入子项目
# 0. 库
add_subdirectory(libraries)

# 1. 可执行程序（Windows && Linux）
add_subdirectory(src/c_cpp/ascii)
add_subdirectory(src/c_cpp/baseconvert)
add_subdirectory(src/c_cpp/binarytree)
add_subdirectory(src/c_cpp/chcolor)
add_subdirectory(src/c_cpp/copyleft)
add_subdirectory(src/c_cpp/cvtcolor)
add_subdirectory(src/c_cpp/digest) # 需要 OpenSSL
add_subdirectory(src/c_cpp/pwdgen)
add_subdirectory(src/c_cpp/use)
add_subdirectory(src/c_cpp/viewfile)

add_subdirectory(src/python) # 复制 py 脚本到安装目录

if (WIN32)          # Only Windows
    # add_subdirectory(src/c_cpp/CodeRainEasyX)
    # add_subdirectory(src/c_cpp/TurnOnRemoteDesktop)
    add_subdirectory(src/c_cpp/cpro)
    add_subdirectory(src/c_cpp/selfdel)
    add_subdirectory(src/c_cpp/setenc)
    add_subdirectory(src/c_cpp/setts)
    add_subdirectory(src/c_cpp/tellme)
    add_subdirectory(src/c_cpp/timer)

    add_subdirectory(src/c_cpp/setenv)     # 根目录的二进制可执行文件
    add_subdirectory(src/windows)          # 复制 bat、ps1 和 vbs 脚本到安装目录
elseif (UNIX)       # Only Linux
    add_subdirectory(src/bash)             # 复制 shell 脚本到安装目录
endif()

