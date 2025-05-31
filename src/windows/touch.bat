@echo off
setlocal

:: 接收路径参数
set Path=%1

if "%Path%"=="" (
    echo Error: 必须提供路径参数
    exit /b 1
)

:: 检查路径是否存在
if exist "%Path%" (
    :: 更新文件的修改时间
    copy /b "%Path%" +,, >nul
) else (
    :: 如果文件不存在，则创建新文件
    type nul > "%Path%"
)

endlocal