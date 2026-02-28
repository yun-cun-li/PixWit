@echo off
REM 关闭所有运行中的CapStep进程

REM 切换到项目根目录
cd /d "%~dp0\.."

echo ========================================
echo 关闭运行中的CapStep进程
echo ========================================
echo.

echo 正在检查CapStep进程...
tasklist /FI "IMAGENAME eq CapStep.exe" 2>NUL | find /I /N "CapStep.exe">NUL

if "%ERRORLEVEL%"=="0" (
    echo 发现运行中的CapStep进程，正在关闭...
    taskkill /F /IM CapStep.exe /T >NUL 2>&1
    
    REM 等待进程完全关闭
    timeout /t 2 /nobreak >NUL
    
    REM 再次检查
    tasklist /FI "IMAGENAME eq CapStep.exe" 2>NUL | find /I /N "CapStep.exe">NUL
    if "%ERRORLEVEL%"=="0" (
        echo [警告] 仍有进程未关闭，请手动关闭后重试
        pause
        exit /b 1
    ) else (
        echo [成功] 所有CapStep进程已关闭
    )
) else (
    echo [信息] 没有发现运行中的CapStep进程
)

echo.
echo ========================================
echo 检查完成
echo ========================================

