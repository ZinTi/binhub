cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED BINHUB_VERSION)
    set(BINHUB_VERSION 0.0.1)  # 独立构建时的默认值
endif()

project(Digest
        VERSION ${BINHUB_VERSION}
        LANGUAGES CXX
        DESCRIPTION "Digest Application"
)

# 设置全局 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置默认构建类型
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# 平台相关设置
set(EXECUTABLE_NAME "digest")

# 直接使用父CMake传入的OpenSSL路径
message(STATUS "Using OpenSSL from: ${LIB_OPENSSL_ROOT}")

# 方法1：先尝试原方法（lib64目录）
set(OpenSSL_DIR "${LIB_OPENSSL_ROOT}/lib64/cmake/OpenSSL")
message(STATUS "Trying OpenSSL_DIR: ${OpenSSL_DIR}")

find_package(OpenSSL 3.5.0 CONFIG QUIET)

# 方法2：如果没找到，尝试lib目录
if(NOT OpenSSL_FOUND)
    set(OpenSSL_DIR "${LIB_OPENSSL_ROOT}/lib/cmake/OpenSSL")
    message(STATUS "OpenSSL not found in lib64, trying: ${OpenSSL_DIR}")
    find_package(OpenSSL 3.5.0 CONFIG QUIET)
endif()

# 方法3：如果前两种都没找到，尝试更通用的搜索（某些系统可能没有cmake子目录）
if(NOT OpenSSL_FOUND)
    # 清理缓存，尝试直接设置OpenSSL根目录
    unset(OpenSSL_DIR CACHE)
    
    # 尝试在常见位置查找
    find_package(OpenSSL 3.5.0 QUIET)
    
    if(NOT OpenSSL_FOUND)
        # 设置环境变量帮助查找
        set(ENV{OPENSSL_ROOT_DIR} "${LIB_OPENSSL_ROOT}")
        find_package(OpenSSL 3.5.0 REQUIRED)
    endif()
endif()

message(STATUS "Found OpenSSL: ${OpenSSL_VERSION}")

# 设置MSVC运行时库 - 这是关键修复
if(MSVC)
    # 使用CMake推荐的方式来设置运行时库
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    else()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    endif()
endif()

# 编译器优化选项
if(MSVC)
    # MSVC编译器选项
    add_compile_options(
        "$<$<CONFIG:Release>:/O2>"
        "$<$<CONFIG:Release>:/W3>"
        "$<$<CONFIG:Release>:/wd4068>"  # 忽略未知的pragma警告
        "$<$<CONFIG:Release>:/wd4267>"  # size_t转换警告
        "$<$<CONFIG:Release>:/wd4244>"  # 类型转换警告
        "$<$<CONFIG:Release>:/wd4996>"  # 不安全函数警告
    )

    # 定义宏来禁用不安全函数警告
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_definitions(_WINSOCK_DEPRECATED_NO_WARNINGS)

    # 对于Debug模式，也添加一些警告抑制
    add_compile_options(
        "$<$<CONFIG:Debug>:/W3>"
        "$<$<CONFIG:Debug>:/wd4068>"
        "$<$<CONFIG:Debug>:/wd4267>"
        "$<$<CONFIG:Debug>:/wd4244>"
        "$<$<CONFIG:Debug>:/wd4996>"
    )

else()
    # GCC/Clang编译器选项
    add_compile_options(
        "$<$<CONFIG:Release>:-Os>"
        "$<$<CONFIG:Release>:-Wall>"
        "$<$<CONFIG:Release>:-Wextra>"
    )
endif()

set(PROJECT_SRC
        src/main.cpp
        src/hash_utils.cpp
)

# 创建可执行文件
add_executable(${PROJECT_NAME}
        ${PROJECT_SRC}
)

# 添加对库的依赖, 优先动态链接binhub库
if(BUILD_SHARED_LIBS)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            binhub_shared
    )
    message(STATUS "Linking with binhub_shared (dynamic)")
elseif (BUILD_STATIC_LIBS)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            binhub_static
    )
    message(STATUS "Linking with binhub_static (static)")
else()
    message(FATAL_ERROR "BUILD_SHARED_LIBS or BUILD_STATIC_LIBS must be set to ON")
endif()

# 设置输出名称
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "${EXECUTABLE_NAME}"
)

# 根据编译器设置不同的链接选项
if(MSVC)
    # MSVC链接选项 - Release模式下优化、去除调试信息
    target_link_options(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Release>:/DEBUG:NONE>"
        "$<$<CONFIG:Release>:/OPT:REF>"
        "$<$<CONFIG:Release>:/OPT:ICF>"
        "$<$<CONFIG:Release>:/LTCG>"
    )
    
    # MSVC特定：确保OpenSSL是静态链接
    # 通过编译选项指定静态链接C运行时库
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE "/MTd")
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE "/MT")
    endif()
    
else()
    if (BUILD_SHARED_LIBS) # 优先使用动态链接
        # 动态链接时
        target_link_options(${PROJECT_NAME} PRIVATE
            "$<$<CONFIG:Release>:-Wl,-s>"
        )
    else()
        # 静态链接时
        target_link_options(${PROJECT_NAME} PRIVATE
            "$<$<CONFIG:Release>:-Wl,-s>"
            "$<$<CONFIG:Release>:-static>"
        )
    endif()
endif()

# 设置包含路径
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 链接OpenSSL库
target_link_libraries(${PROJECT_NAME} PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
)

# 添加平台特定的链接库
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            ws2_32
            user32
            advapi32
            crypt32
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
            dl
            pthread
    )
endif()

# 安装配置
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 使用方式示例

# Linux/MacOS (标准安装)
# cmake -S . -B build

# Linux/MacOS (自定义路径)
# OPENSSL_ROOT_DIR=/usr/local/openssl-3.5.0 cmake -S . -B build

# Windows (自定义路径)
# cmake -G "MinGW Makefiles" -S . -B build -DOPENSSL_ROOT_DIR=D:/build/openssl-win64-mingw

# 如果 OpenSSL 提供了 Config 文件
# cmake -S . -B build -DOpenSSL_DIR=/path/to/openssl/cmake/directory
