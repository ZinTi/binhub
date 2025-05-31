@echo off
setlocal enabledelayedexpansion

REM 获取文件后缀名
for %%A in (%*) do (
    set "filename=%%~xA"
    if "!filename!"==".c" (
        echo Are you sure you want to delete the file: %%A? (Y/N)
        set /p confirm=
        if "!confirm!"=="Y" (
            DEL "%%A"
        ) else (
            echo Delete operation cancelled.
        )
    ) else if "!filename!"==".cpp" (
        echo Are you sure you want to delete the file: %%A? (Y/N)
        set /p confirm=
        if "!confirm!"=="Y" (
            DEL "%%A"
        ) else (
            echo Delete operation cancelled.
        )
    ) else if "!filename!"==".h" (
        echo Are you sure you want to delete the file: %%A? (Y/N)
        set /p confirm=
        if "!confirm!"=="Y" (
            DEL "%%A"
        ) else (
            echo Delete operation cancelled.
        )
    ) else if "!filename!"==".txt" (
        echo Are you sure you want to delete the file: %%A? (Y/N)
        set /p confirm=
        if "!confirm!"=="Y" (
            DEL "%%A"
        ) else (
            echo Delete operation cancelled.
        )
    ) else if "!filename!"==".java" (
        echo Are you sure you want to delete the file: %%A? (Y/N)
        set /p confirm=
        if "!confirm!"=="Y" (
            DEL "%%A"
        ) else (
            echo Delete operation cancelled.
        )
    ) else if "!filename!"==".csv" (
        echo Are you sure you want to delete the file: %%A? (Y/N)
        set /p confirm=
        if "!confirm!"=="Y" (
            DEL "%%A"
        ) else (
            echo Delete operation cancelled.
        )
    ) else if "!filename!"==".sql" (
        echo Are you sure you want to delete the file: %%A? (Y/N)
        set /p confirm=
        if "!confirm!"=="Y" (
            DEL "%%A"
        ) else (
            echo Delete operation cancelled.
        )
    ) else (
        DEL "%%A"
    )
)
endlocal