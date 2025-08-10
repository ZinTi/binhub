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

# 改进的 OpenSSL 查找逻辑，同时支持 Module 和 Config 模式
# 1. 首先尝试通过环境变量或CMake变量指定路径
if(DEFINED ENV{OPENSSL_ROOT_DIR} AND NOT OPENSSL_ROOT_DIR)
    set(OPENSSL_ROOT_DIR "$ENV{OPENSSL_ROOT_DIR}" CACHE PATH "OpenSSL root directory")
endif()

if(DEFINED ENV{OPENSSL_DIR} AND NOT OpenSSL_DIR)
    set(OpenSSL_DIR "$ENV{OPENSSL_DIR}" CACHE PATH "OpenSSL config directory")
endif()

# 2. 设置搜索路径
set(OPENSSL_SEARCH_PATHS)
if(OPENSSL_ROOT_DIR)
    message(STATUS "Using custom OpenSSL path: ${OPENSSL_ROOT_DIR}")
    list(APPEND OPENSSL_SEARCH_PATHS ${OPENSSL_ROOT_DIR})
endif()

# 添加常见 OpenSSL 安装路径
list(APPEND OPENSSL_SEARCH_PATHS
    "/usr/local/opt/openssl"  # Homebrew on macOS
    "/opt/homebrew/opt/openssl"  # Homebrew on Apple Silicon
    "C:/OpenSSL"  # Windows common install path
    "C:/OpenSSL-Win64"  # Windows 64-bit
    "C:/OpenSSL-Win32"  # Windows 32-bit
)

# 3. 首先尝试使用 Config 模式查找 OpenSSL
set(OpenSSL_FOUND_CONFIG FALSE)
if(OpenSSL_DIR OR OPENSSL_SEARCH_PATHS)
    find_package(OpenSSL 3.0.0 QUIET
        CONFIG
        HINTS ${OpenSSL_DIR}
        PATHS ${OPENSSL_SEARCH_PATHS}
        PATH_SUFFIXES lib/cmake/openssl lib/cmake/OpenSSL
    )
    if(OpenSSL_FOUND)
        set(OpenSSL_FOUND_CONFIG TRUE)
        message(STATUS "Found OpenSSL via Config mode: ${OpenSSL_VERSION}")
    endif()
endif()

# 4. 如果 Config 模式失败，尝试使用 Module 模式
if(NOT OpenSSL_FOUND)
    message(STATUS "OpenSSL Config mode not found, trying Module mode...")
    
    # 设置 Module 模式查找的提示路径
    if(OPENSSL_ROOT_DIR)
        set(CMAKE_PREFIX_PATH_SAVE ${CMAKE_PREFIX_PATH})
        set(CMAKE_PREFIX_PATH ${OPENSSL_ROOT_DIR} ${CMAKE_PREFIX_PATH})
    endif()
    
    # 尝试 Module 模式查找
    find_package(OpenSSL 3.0.0 MODULE)
    
    # 恢复 CMAKE_PREFIX_PATH
    if(OPENSSL_ROOT_DIR)
        set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH_SAVE})
    endif()
endif()

# 5. 如果仍然找不到，尝试使用 pkg-config (Unix-like 系统)
if(NOT OpenSSL_FOUND AND UNIX)
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PC_OPENSSL QUIET openssl)
        if(PC_OPENSSL_FOUND)
            message(STATUS "Found OpenSSL via pkg-config: ${PC_OPENSSL_VERSION}")
            
            # 创建自定义目标来模拟 OpenSSL::SSL 和 OpenSSL::Crypto
            add_library(OpenSSL::SSL UNKNOWN IMPORTED)
            set_target_properties(OpenSSL::SSL PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${PC_OPENSSL_INCLUDE_DIRS}"
                IMPORTED_LOCATION "${PC_OPENSSL_LIBRARIES}"
            )
            
            add_library(OpenSSL::Crypto UNKNOWN IMPORTED)
            set_target_properties(OpenSSL::Crypto PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${PC_OPENSSL_INCLUDE_DIRS}"
                IMPORTED_LOCATION "${PC_OPENSSL_LIBRARIES}"
            )
            
            set(OpenSSL_FOUND TRUE)
            set(OpenSSL_VERSION ${PC_OPENSSL_VERSION})
        endif()
    endif()
endif()

# 6. 如果仍然找不到，提供更详细的错误信息
if(NOT OpenSSL_FOUND)
    message(FATAL_ERROR "
OpenSSL not found. Please ensure OpenSSL 3.0.0 or later is installed.

Possible solutions:
1. Install OpenSSL development packages:
   - Fedora/RHEL: sudo dnf install openssl-devel
   - Ubuntu/Debian: sudo apt-get install libssl-dev
   - macOS with Homebrew: brew install openssl

2. If OpenSSL is installed in a custom location, set one of these variables:
   - OPENSSL_ROOT_DIR: /path/to/openssl
   - OpenSSL_DIR: /path/to/openssl/cmake/directory

3. For Windows, download OpenSSL from:
   - https://slproweb.com/products/Win32OpenSSL.html

Current search paths: ${CMAKE_PREFIX_PATH}
")
else()
    message(STATUS "Found OpenSSL: ${OpenSSL_VERSION}")
endif()

# 使用现代编译选项设置方式
add_compile_options(
        "$<$<CONFIG:Release>:-Os>"
        "$<$<CONFIG:Release>:-Wall>"
        "$<$<CONFIG:Release>:-Wextra>"
)

set(PROJECT_SRC
        src/main.cpp
        src/hash_utils.cpp
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${PROJECT_SRC})

# 设置输出名称和链接选项
set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "${EXECUTABLE_NAME}"
        LINK_OPTIONS "$<$<CONFIG:Release>:-Wl,-s>"
)

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
    # Windows静态链接
    set_target_properties(${PROJECT_NAME} PROPERTIES
            LINK_FLAGS "-static"
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
