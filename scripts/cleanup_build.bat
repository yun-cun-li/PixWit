@echo off
chcp 65001 >nul
echo ========================================
echo 清理 build 目录
echo ========================================
echo.

REM 切换到项目根目录
cd /d "%~dp0\.."

echo [1/4] 关闭所有 CapStep 进程...
taskkill /F /IM CapStep.exe /T >NUL 2>&1
timeout /t 1 /nobreak >NUL

echo [2/4] 关闭可能占用 build 目录的进程...
taskkill /F /IM msbuild.exe /T >NUL 2>&1
taskkill /F /IM cmake.exe /T >NUL 2>&1
timeout /t 1 /nobreak >NUL

echo [3/4] 等待文件系统释放锁...
timeout /t 2 /nobreak >NUL

echo [4/4] 清理 build 目录...
if exist "build" (
    rd /s /q "build" 2>NUL
    if exist "build" (
        echo [警告] 无法完全删除 build 目录
        echo [提示] 请手动关闭所有占用文件的程序，然后重试
        echo [建议] 关闭 Visual Studio、任务管理器等可能占用文件的程序
        pause
        exit /b 1
    ) else (
        echo [成功] build 目录已清理
    )
) else (
    echo [信息] build 目录不存在
)

echo.
echo ========================================
echo 清理完成！现在可以重新编译
echo ========================================
echo.
pause

