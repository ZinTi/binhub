cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED BINHUB_VERSION)
    set(BINHUB_VERSION 0.0.1)  # 独立构建时的默认值
endif()

project(Use
        VERSION ${BINHUB_VERSION}
        LANGUAGES CXX
        DESCRIPTION "Use Application"
)

# 设置全局 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 必须支持指定标准
set(CMAKE_CXX_EXTENSIONS OFF)        # 禁用编译器扩展

# 设置默认构建类型
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# 平台相关设置
set(EXECUTABLE_NAME "use")  # 所有平台使用相同的名称

# 使用现代编译选项设置方式
add_compile_options(
        "$<$<CONFIG:Release>:-Os>"
        "$<$<CONFIG:Release>:-Wall>"
        "$<$<CONFIG:Release>:-Wextra>"
)

# 创建可执行文件
add_executable(${PROJECT_NAME}
        use.cpp
)

# 添加对库的依赖, 默认动态库
if(BUILD_SHARED_LIBS)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            binhub_shared
    )
elseif (BUILD_STATIC_LIBS)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            binhub_static
    )
else()
    message(FATAL_ERROR "BUILD_SHARED_LIBS or BUILD_STATIC_LIBS must be set to ON")
endif()

# 设置输出名称和链接选项
set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "${EXECUTABLE_NAME}"
        LINK_OPTIONS "$<$<CONFIG:Release>:-Wl,-s>"
)

# 设置包含路径
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 安装配置
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
