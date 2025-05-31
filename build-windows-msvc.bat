@echo off
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl
cmake --build build --config Release --parallel %NUMBER_OF_PROCESSORS%
