cmake_minimum_required(VERSION 3.20)
project(binhub VERSION 0.0.1)  # 版本号

# 定义统一版本号变量（所有子项目共享）
set(BINHUB_VERSION ${PROJECT_VERSION})

# 使用UTC时间
string(TIMESTAMP UTC_DATE "%Y%m%d" UTC)

# 设置平台信息（全小写）
# 项目名称-v版本号-编译日期-操作系统-处理器架构-编译器
string(TOLOWER
    "${PROJECT_NAME}-v${PROJECT_VERSION}-${UTC_DATE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_C_COMPILER_ID}" INSTALL_ROOT_DIR
)

# 设置所有库和可执行文件的默认输出目录（若子项目没有设置输出目录，则使用此默认目录）
if(WIN32)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${INSTALL_ROOT_DIR}/lib) # 静态库 (.a, .lib) 和 导入库 (.dll.a, .lib)
    # set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${INSTALL_ROOT_DIR}/bin) # 对 Windows 无效
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${INSTALL_ROOT_DIR}/bin) # 运行时文件 (.exe, .dll)
elseif (UNIX)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${INSTALL_ROOT_DIR}/lib) # 静态库 (.a)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${INSTALL_ROOT_DIR}/bin) # 动态库 (.so)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${INSTALL_ROOT_DIR}/bin) # 可执行文件
else()
    message(WARNING "Unknown platform")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${INSTALL_ROOT_DIR}/unknown)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${INSTALL_ROOT_DIR}/unknown)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${INSTALL_ROOT_DIR}/unknown)
endif()

# 设置第三方库路径
set(LIB_OPENSSL_ROOT "${CMAKE_SOURCE_DIR}/third_party/openssl")

# 引入子项目
# 0. 库
add_subdirectory(lib)

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
    add_subdirectory(src/c_cpp/selfdel)
    add_subdirectory(src/c_cpp/setenc)
    add_subdirectory(src/c_cpp/setts)
    add_subdirectory(src/c_cpp/tellme)
    add_subdirectory(src/c_cpp/timer)

    add_subdirectory(src/c_cpp/setenv)     # 根目录的二进制可执行文件
    add_subdirectory(src/windows)          # 复制 bat、ps1 和 vbs 脚本到安装目录

    if(MSVC)

    else() # MinGW
        add_subdirectory(src/c_cpp/cpro)
    endif()
elseif (UNIX)       # Only Linux
    add_subdirectory(src/bash)             # 复制 shell 脚本到安装目录
endif()

