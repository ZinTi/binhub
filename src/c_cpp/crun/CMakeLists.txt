cmake_minimum_required(VERSION 3.30)
project(crun)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_SRC
        # 1. 入口点 && project
        src/main.cpp
        src/common/SLLC.cpp
        src/common/SLLC.h
        src/common/SLLS.cpp
        src/common/SLLS.h
        src/common/ProjectCommon.cpp
        src/common/ProjectCommon.h
        src/common/info.cpp
        src/common/info.h

        # 3.
        src/config_loader.cpp
        src/config_loader.h
        src/crepl.cpp
        src/crepl.h
        src/exception.cpp
        src/exception.h
        src/main.cpp
        src/match_regex.cpp
        src/match_regex.h
        src/prog.cpp
        src/prog.h
        src/resource.h
        src/safe_exit.cpp
        src/safe_exit.h
        src/slls_ext.cpp
        src/slls_ext.h
        src/str.cpp
        src/str.h
        src/subprocess.cpp
        src/subprocess.h
        src/use_template.cpp
        src/use_template.h
)

add_executable(crun ${PROJECT_SRC})

target_include_directories(crun
        PRIVATE
        src
        src/common
)
