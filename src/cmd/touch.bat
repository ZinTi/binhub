@echo off
setlocal

:: ����·������
set Path=%1

if "%Path%"=="" (
    echo Error: �����ṩ·������
    exit /b 1
)

:: ���·���Ƿ����
if exist "%Path%" (
    :: �����ļ����޸�ʱ��
    copy /b "%Path%" +,, >nul
) else (
    :: ����ļ������ڣ��򴴽����ļ�
    type nul > "%Path%"
)

endlocal