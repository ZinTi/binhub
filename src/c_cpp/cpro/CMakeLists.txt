# 以前学 C 时写的用于运行内存中的代码的 REPL 小工具，计划用 C++ 完全重写
cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED BINHUB_VERSION)
    set(BINHUB_VERSION 0.0.1)  # 独立构建时的默认值
endif()

project(CPro
        VERSION ${BINHUB_VERSION}
        LANGUAGES CXX
        DESCRIPTION "CPro Application"
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
set(EXECUTABLE_NAME "cpro")  # 所有平台使用相同的名称

# 使用现代编译选项设置方式
add_compile_options(
        "$<$<CONFIG:Release>:-Os>"
        "$<$<CONFIG:Release>:-Wall>"
        "$<$<CONFIG:Release>:-Wextra>"
)

set(PROJECT_SRC
        # 0. 入口点
        src/main.cpp

        # 1. common
        src/common/text_editor.cpp      # 命令行文本编辑控件
        src/common/table.cpp
        src/common/gcc_option.cpp
        src/common/gen_template.cpp
        src/common/match_regex.cpp
        src/common/memory_gcc_compiler.cpp
        src/common/str.cpp

        # 2. proj
        src/proj/config_loader.cpp
        src/proj/proj_info.cpp
        src/proj/help.cpp

        # 3. mod_proj
        src/mod_proj/mod_proj.cpp

        # 4. mod_repl
        # src/mod_repl/repl.cpp
        src/mod_repl/memory_prog.cpp
        src/mod_repl/memory_header.cpp
        src/mod_repl/memory_macro.cpp
        src/mod_repl/memory_function.cpp
        src/mod_repl/memory_struct.cpp
        src/mod_repl/memory_union.cpp

        # 5. mod_run
        src/mod_run/handle_cmd_run.cpp
        src/mod_run/subprocess.cpp

        # 6. mod_scaffold
        src/mod_scaffold/scaffold.cpp
        src/mod_scaffold/fixed_template.cpp

        # 7. mod_winres
        src/mod_winres/handle_cmd_winres.cpp
        src/mod_winres/win_res.cpp

)

# 创建可执行文件
add_executable(${PROJECT_NAME}
        ${PROJECT_SRC}
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
        ${CMAKE_CURRENT_SOURCE_DIR}/include/common
        ${CMAKE_CURRENT_SOURCE_DIR}/include/proj
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mod_proj
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mod_repl
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mod_run
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mod_scaffold
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mod_winres
)

# 安装配置
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


