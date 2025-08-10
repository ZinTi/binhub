@echo off
setlocal

:: 设置文件路径
set filePath1=%BINHUB_HOME%\res\text\manual
set filePath2=%BINHUB_HOME%\res\text\version

:: 输出标题
echo Command Manual

:: 检查并读取第一个文件
if exist "%filePath1%" (
    type "%filePath1%"
) else (
    echo File not found: %filePath1%
)

:: 检查并读取第二个文件
if exist "%filePath2%" (
    type "%filePath2%"
) else (
    echo File not found: %filePath2%
)

endlocal