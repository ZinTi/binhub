cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED BINHUB_VERSION)
    set(BINHUB_VERSION 0.0.1)  # 独立构建时的默认值
endif()

project(SetFileEnc
        VERSION ${BINHUB_VERSION}
        LANGUAGES CXX
        DESCRIPTION "SetFileEnc Application"
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
set(EXECUTABLE_NAME "setenc")  # 所有平台使用相同的名称

# 编译器优化选项
if(MSVC)
    # MSVC编译器选项
    add_compile_options(
        "$<$<CONFIG:Release>:/O2>"
        "$<$<CONFIG:Release>:/W3>"
        "$<$<CONFIG:Release>:/wd4068>"  # 忽略未知的pragma警告
        # 添加MSVC特有的警告抑制
        "$<$<CONFIG:Release>:/wd4267>"  # size_t转换警告
        "$<$<CONFIG:Release>:/wd4244>"  # 类型转换警告
        "$<$<CONFIG:Release>:/wd4996>"  # 不安全函数警告
    )

    # 定义宏来禁用不安全函数警告
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_definitions(_WINSOCK_DEPRECATED_NO_WARNINGS)

else()
    # GCC/Clang编译器选项
    add_compile_options(
        "$<$<CONFIG:Release>:-Os>"
        "$<$<CONFIG:Release>:-Wall>"
        "$<$<CONFIG:Release>:-Wextra>"
    )
endif()

# 创建可执行文件
add_executable(${PROJECT_NAME}
        main.cpp
)

# 添加对库的依赖, 默认链接binhub动态库
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

# 安装配置
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)