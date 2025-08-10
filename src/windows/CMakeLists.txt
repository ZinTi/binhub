# 复制此目录下的所有 .bat .ps1 .vbs 等脚本到安装目录的 bin 目录下
cmake_minimum_required(VERSION 3.20)

# 获取当前目录下所有需要复制的文件
file(GLOB windows_script_files
        LIST_DIRECTORIES false
        "*.bat" "*.ps1" "*.vbs" "*.lnk"
)

# 仅当找到匹配文件时才创建复制任务
if(windows_script_files)
    # 添加自定义目标，在构建时执行复制操作
    add_custom_target(CopyScriptFilesForWindows ALL
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${windows_script_files}
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Copying script files to output directory"
            VERBATIM
    )
else()
    message(STATUS "No script files found to copy in ${CMAKE_CURRENT_SOURCE_DIR}")
endif()
