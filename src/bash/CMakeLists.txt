# 复制此目录下的所有 bash 脚本到安装目录的 bin 目录下
cmake_minimum_required(VERSION 3.20)
project(BashScriptCopy)

# 获取当前目录下所有文件（排除CMakeLists.txt自身）
file(GLOB ALL_FILES
        LIST_DIRECTORIES false
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        "*"
)
list(REMOVE_ITEM ALL_FILES "CMakeLists.txt")

# 创建自定义目标实现文件复制
add_custom_target(CopyScriptFilesForLinux ALL
        COMMENT "Copying project files to runtime directory"
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_COMMAND} -E copy
        ${ALL_FILES}
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# 设置目标属性确保在构建时执行
set_target_properties(CopyScriptFilesForLinux PROPERTIES
        EXCLUDE_FROM_ALL FALSE
        FOLDER "Utilities"
)