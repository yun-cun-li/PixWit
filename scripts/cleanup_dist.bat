@echo off
chcp 65001 >nul
echo ========================================
echo 清理 dist 目录和文件锁
echo ========================================
echo.

REM 切换到项目根目录
cd /d "%~dp0\.."

echo [1/5] 关闭所有 CapStep 进程...
taskkill /F /IM CapStep.exe /T >NUL 2>&1
timeout /t 2 /nobreak >NUL

echo [2/5] 关闭访问 dist 目录的文件管理器窗口...
taskkill /FI "WINDOWTITLE eq dist*" /IM explorer.exe 2>NUL >NUL
timeout /t 2 /nobreak >NUL

echo [3/5] 删除旧的安装程序和压缩包...
del /F /Q "dist\*.exe" 2>NUL
del /F /Q "dist\*.zip" 2>NUL
timeout /t 1 /nobreak >NUL

echo [4/5] 等待文件系统释放锁...
timeout /t 3 /nobreak >NUL

echo [5/5] 清理 dist 目录...
if exist "dist" (
    rd /s /q "dist" 2>NUL
    if exist "dist" (
        echo [警告] 无法完全删除 dist 目录
        echo [提示] 请手动关闭所有占用文件的程序，然后重试
        pause
        exit /b 1
    ) else (
        echo [成功] dist 目录已清理
    )
) else (
    echo [信息] dist 目录不存在
)

echo.
echo ========================================
echo 清理完成！现在可以重新运行 release.bat
echo ========================================
echo.
pause

