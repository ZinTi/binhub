@echo off
setlocal

:: 接收文件名和算法参数
set fileName=%1
set algorithm="SHA512"

:: 检查是否提供了文件名和算法
if "%fileName%"=="" (
    echo Error: 必须提供文件名
    exit /b 1
)

if "%algorithm%"=="" (
    echo Error: 必须提供哈希算法（如 MD5、SHA1、SHA256）
    exit /b 1
)

:: 计算并显示文件的哈希值
certutil -hashfile "%fileName%" %algorithm%

endlocal